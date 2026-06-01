// Game scene - core gameplay with tutorial and hero animation
const {
  DESIGN_W, DESIGN_H,
  GRID_COLS, GRID_ROWS, TILE_W, TILE_H,
  HERO_START, HERO_INIT_BLOOD, MAX_MONSTERS, MONSTER_CHASE_DIST,
  BARRIER, PROP, MONSTER, PROP_SCORE, MONSTER_SCORE,
  LEVELS, GAME_MODE,
} = require('./config')
const { AStar } = require('./astar')

const GRID_ORIGIN_X = (DESIGN_W - GRID_COLS * TILE_W) / 2
const GRID_ORIGIN_Y = 100

// Tutorial state machine (matches original CGuidLayer)
const TUT = {
  START: 0, MOVETOBOX0: 1, PUSHBOX0: 2, MOVETOBOX1: 3,
  BREAKBOX: 4, ADDBOMB: 5, ADDPROP: 6, CLICKPROP: 7,
  MOVETOBOX2: 8, WINGAME: 9,
}

// Where the player must tap for each tutorial state
const TUT_TARGETS = {
  [TUT.START]:      { col: 2, row: 8 },
  [TUT.MOVETOBOX0]: { col: 2, row: 7 },
  [TUT.PUSHBOX0]:   { col: 2, row: 3 },
  [TUT.MOVETOBOX1]: { col: 2, row: 2 },
  [TUT.BREAKBOX]:   { col: 2, row: 4 },
  [TUT.ADDBOMB]:    { col: 3, row: 5 },
  // ADDPROP: auto-advance, no tap
  [TUT.CLICKPROP]:  { col: 4, row: 4 },
  [TUT.MOVETOBOX2]: { col: 4, row: 3 },
}

// Hero walk frame pairs: [standing, push]
const HERO_FRAMES = {
  down:  ['hero_down',           'hero_pushdown'],
  up:    ['hero_up',             'hero_pushup'],
  left:  ['hero_standby',        'hero_pushhorizontal'],
  right: ['hero_standbyright',   'hero_pushright'],
}

class GameScene {
  constructor(rm, audio, storage, onNavigate) {
    this.rm = rm
    this.audio = audio
    this.storage = storage
    this.onNavigate = onNavigate
    this.levelNum = 1
  }

  show(params) {
    this.levelNum = (params && params.level) || 1
    this.data = this.storage.loadGameData()
    this.audio.stopBGM()
    this.initLevel()
    this.audio.playSound('readygo')
  }

  hide() {
    this.clearTimers()
  }

  initLevel() {
    this.clearTimers()
    this.state = 'playing'
    this.score = 0
    this.blood = this.data.blood || HERO_INIT_BLOOD
    this.boxProp = this.data.boxProp || 0
    this.propGetCount = 0
    this.boxDestroyCount = 0
    this.stopHeroCheck = false
    this.heroFacing = 'left'

    // Walking animation
    this.heroWalkFrame = 0   // 0=standing, 1=push
    this.heroWalkTimer = 0

    // Grid
    this.grid = []
    this.barriers = []
    this.monsters = []
    this.props = []
    this.hero = { col: HERO_START.col, row: HERO_START.row, animX: 0, animY: 0 }
    this.heroPath = []
    this.heroMoving = false
    this.heroAnim = null
    this.floatScores = []
    this.anims = []

    this.monsterQueue = []
    this.monsterSpawnIndex = 0
    this.monsterSpawnTimer = 0

    // Tutorial
    this.gameMode = (this.levelNum === 1) ? GAME_MODE.TUTORIAL : GAME_MODE.NORMAL
    this.tutorialState = -1
    this.handVisible = false
    this.handCol = 0
    this.handRow = 0
    this.handTimer = 0
    this.handBob = 0

    const levelData = this.getLevelData(this.levelNum)
    if (!levelData) {
      this.generateLevel()
    } else {
      this.parseLevelData(levelData)
    }

    this.monsterSpawnInterval = 1000

    if (this.gameMode === GAME_MODE.TUTORIAL) {
      this.initTutorial()
    } else {
      this.initPropIndexes()
    }
  }

  // ---- Tutorial ----
  initTutorial() {
    this.gameMode = GAME_MODE.TUTORIAL
    this.tutorialState = TUT.START
    this.monsterQueue = [] // tutorial controls its own monsters

    // Place 4 test monsters (BugTestMonster - static, don't move)
    const testPositions = [[2,4],[5,6],[7,1],[6,3]]
    for (const [c, r] of testPositions) {
      const pos = this.gridToScreen(c, r)
      this.monsters.push({ col: c, row: r, type: MONSTER.BUG, animX: pos.x, animY: pos.y, static: true })
    }

    // Show hand at first target
    this.showHand(2, 8)
  }

  showHand(col, row) {
    this.handVisible = true
    this.handCol = col
    this.handRow = row
    this.handTimer = 0
  }

  hideHand() {
    this.handVisible = false
  }

  advanceTutorial() {
    if (this.gameMode !== GAME_MODE.TUTORIAL) return
    const prev = this.tutorialState
    this.tutorialState++

    switch (this.tutorialState) {
      case TUT.MOVETOBOX0: // After START: hero moved to (2,8), show hand at box (2,7)
        this.hideHand()
        this.scheduleOnce(() => this.showHand(2, 7), 1000)
        break

      case TUT.PUSHBOX0: // After MOVETOBOX0: box pushed, show hand at (2,3)
        this.hideHand()
        this.scheduleOnce(() => this.showHand(2, 3), 1000)
        break

      case TUT.MOVETOBOX1: // After PUSHBOX0: hero moved to (2,3), show hand at (2,2)
        this.hideHand()
        this.scheduleOnce(() => this.showHand(2, 2), 1000)
        break

      case TUT.BREAKBOX: // After MOVETOBOX1: spawn props
        this.spawnTutorialProps()
        this.hideHand()
        this.scheduleOnce(() => this.showHand(2, 4), 1000)
        break

      case TUT.ADDBOMB: // After BREAKBOX: collected bomb, add monster + show hand at cherry
        this.scheduleOnce(() => {
          const pos = this.gridToScreen(4, 0)
          this.monsters.push({ col: 4, row: 0, type: MONSTER.BUG, animX: pos.x, animY: pos.y, static: true })
        }, 1000)
        this.hideHand()
        this.scheduleOnce(() => this.showHand(3, 5), 1000)
        break

      case TUT.ADDPROP: // After ADDBOMB: collected cherry, add new boxes
        this.hideHand()
        this.addTutorialBoxes()
        this.scheduleOnce(() => this.showHand(4, 4), 500)
        // Auto-advance to CLICKPROP
        this.tutorialState = TUT.CLICKPROP
        break

      case TUT.MOVETOBOX2: // After CLICKPROP: show hand at (4,3)
        this.showHand(4, 3)
        break

      case TUT.WINGAME:
        this.hideHand()
        this._waitingForTutorialEnd = true
        break
    }
  }

  spawnTutorialProps() {
    const propData = [
      { col: 2, row: 4, type: PROP.BOMB },
      { col: 3, row: 5, type: PROP.CHERRY },
    ]
    for (const p of propData) {
      const pos = this.gridToScreen(p.col, p.row)
      this.props.push({ col: p.col, row: p.row, type: p.type, animX: pos.x, animY: pos.y, blink: true, blinkTimer: 0 })
    }
  }

  addTutorialBoxes() {
    const positions = [[2,5],[4,3],[7,3]]
    for (const [c, r] of positions) {
      if (this.grid[r][c] !== 0) continue
      this.grid[r][c] = BARRIER.BOX
      const pos = this.gridToScreen(c, r)
      this.barriers.push({ col: c, row: r, type: BARRIER.BOX, animX: pos.x, animY: pos.y, blink: true, blinkTimer: 0 })
    }
  }

  onTutorialWin() {
    if (this.state !== 'playing') return
    this.state = 'win'
    this.audio.playSound('win')
    const stars = 3
    this.data.levelStars[1] = stars
    this.data.unlockedLevel = Math.max(this.data.unlockedLevel, 2)
    this.data.blood = this.blood
    this.data.currentLevel = 2
    this.storage.saveGameData(this.data)
  }

  // Simple scheduler for tutorial delays
  scheduleOnce(callback, delay) {
    this._pendingSchedules = this._pendingSchedules || []
    this._pendingSchedules.push({ callback, delay, elapsed: 0 })
  }

  updateSchedules(dt) {
    if (!this._pendingSchedules) return
    for (let i = this._pendingSchedules.length - 1; i >= 0; i--) {
      this._pendingSchedules[i].elapsed += dt
      if (this._pendingSchedules[i].elapsed >= this._pendingSchedules[i].delay) {
        this._pendingSchedules[i].callback()
        this._pendingSchedules.splice(i, 1)
      }
    }
  }

  // ---- Level loading ----
  getLevelData(num) {
    if (num <= LEVELS.length) return LEVELS[num - 1]
    return null
  }

  generateLevel() {
    this.grid = Array.from({ length: GRID_ROWS }, () => Array(GRID_COLS).fill(0))
    const boxCount = Math.min(3 + Math.floor(this.levelNum / 5), 10)
    const stoneCount = Math.min(2 + Math.floor(this.levelNum / 8), 6)
    for (let i = 0; i < boxCount; i++) {
      let c, r
      do { c = Math.floor(Math.random() * GRID_COLS); r = Math.floor(Math.random() * (GRID_ROWS - 2)) + 1 }
      while (this.grid[r][c] !== 0 || (c === HERO_START.col && r === HERO_START.row))
      this.grid[r][c] = BARRIER.BOX
      this.barriers.push({ col: c, row: r, type: BARRIER.BOX, animX: 0, animY: 0 })
    }
    for (let i = 0; i < stoneCount; i++) {
      let c, r
      do { c = Math.floor(Math.random() * GRID_COLS); r = Math.floor(Math.random() * GRID_ROWS) }
      while (this.grid[r][c] !== 0 || (c === HERO_START.col && r === HERO_START.row))
      this.grid[r][c] = BARRIER.STONE
      this.barriers.push({ col: c, row: r, type: BARRIER.STONE, animX: 0, animY: 0 })
    }
    const monsterCount = Math.min(10 + Math.floor(this.levelNum / 3), 25)
    for (let i = 0; i < monsterCount; i++) this.monsterQueue.push(6 + Math.floor(Math.random() * 3))
  }

  parseLevelData(levelData) {
    this.grid = Array.from({ length: GRID_ROWS }, () => Array(GRID_COLS).fill(0))
    for (let r = 0; r < GRID_ROWS; r++) {
      for (let c = 0; c < GRID_COLS; c++) {
        const t = levelData.map[r][c]
        this.grid[r][c] = t
        if (t >= BARRIER.BOX && t <= BARRIER.BIGTREE) {
          this.barriers.push({ col: c, row: r, type: t, animX: 0, animY: 0 })
        }
      }
    }
    this.monsterQueue = levelData.monsters || []
  }

  clearTimers() {
    this._pendingSchedules = []
    if (this._timeouts) { for (const id of this._timeouts) clearTimeout(id); this._timeouts = [] }
  }

  // ---- Prop trigger system (matches C++ ZoneLayer::InitProp / OnAddProp) ----
  initPropIndexes() {
    const total = this.barriers.length
    if (total < 3) {
      this.propIndexes = [-1, -1]
      return
    }
    let idx0, idx1
    do {
      idx0 = total - (Math.floor(Math.random() * 3) + 1)
      idx1 = total - (Math.floor(Math.random() * 3) + 3)
    } while (idx0 === idx1 || idx0 < 0 || idx1 < 0)
    this.propIndexes = [idx0, idx1]
  }

  onPropTrigger() {
    if (this.gameMode !== GAME_MODE.NORMAL) return
    for (let i = 0; i < 2; i++) {
      if (this.propIndexes[i] === this.barriers.length) {
        this.propIndexes[i] = -1 // mark as used
        // Pick 2 random different prop types
        const allTypes = [PROP.BOMB, PROP.CHERRY, PROP.REDHEART, PROP.SHOES]
        let t0, t1
        do {
          t0 = allTypes[Math.floor(Math.random() * allTypes.length)]
          t1 = allTypes[Math.floor(Math.random() * allTypes.length)]
        } while (t0 === t1)
        const types = [t0, t1]

        // Fill props up to 2
        while (this.props.length < 2) {
          const type = types[this.props.length]
          // Find random unoccupied position
          let col, row
          do {
            col = Math.floor(Math.random() * (GRID_COLS - 2)) + 1
            row = Math.floor(Math.random() * (GRID_ROWS - 3)) + 2
          } while (this.isBlocked(col, row) || (col === this.hero.col && row === this.hero.row))

          const pos = this.gridToScreen(col, row)
          this.props.push({ col, row, type, animX: pos.x, animY: pos.y, blink: true, blinkTimer: 0 })
        }
      }
    }
  }

  // ---- Coordinate conversion ----
  gridToScreen(col, row) {
    return { x: GRID_ORIGIN_X + col * TILE_W, y: GRID_ORIGIN_Y + (GRID_ROWS - 1 - row) * TILE_H }
  }

  screenToGrid(sx, sy) {
    return { col: Math.floor((sx - GRID_ORIGIN_X) / TILE_W), row: GRID_ROWS - 1 - Math.floor((sy - GRID_ORIGIN_Y) / TILE_H) }
  }

  isValidGrid(col, row) { return col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS }
  isBlocked(col, row) { return !this.isValidGrid(col, row) || this.grid[row][col] !== 0 }
  setFacing(dx, dy) {
    if (dx < 0) this.heroFacing = 'left'
    else if (dx > 0) this.heroFacing = 'right'
    else if (dy > 0) this.heroFacing = 'up'
    else if (dy < 0) this.heroFacing = 'down'
  }

  getBarrierAt(col, row) { return this.barriers.find(b => b.col === col && b.row === row) }
  getMonsterAt(col, row) { return this.monsters.find(m => m.col === col && m.row === row) }

  getObstacles() {
    const obs = new Set()
    for (const b of this.barriers) obs.add(b.col + ',' + b.row)
    for (const m of this.monsters) obs.add(m.col + ',' + m.row)
    return obs
  }

  // ---- Touch ----
  onTouchStart(x, y) {}

  onTouchEnd(x, y) {
    if (this.state !== 'playing') {
      this.handleResultTouch(x, y)
      return
    }

    // Pause button
    if (x >= DESIGN_W - 90 && x <= DESIGN_W - 10 && y >= 20 && y <= 100) {
      this.audio.playSound('box')
      this.onNavigate('setting', { fromGame: true, level: this.levelNum })
      return
    }

    // Add box button
    if (this.boxProp > 0) {
      const addBtnX = DESIGN_W - 110, addBtnY = DESIGN_H - 80
      if (x >= addBtnX && x <= addBtnX + 90 && y >= addBtnY && y <= addBtnY + 60) {
        this.audio.playSound('box')
        this.addBoxAtRandom()
        return
      }
    }

    const { col, row } = this.screenToGrid(x, y)
    if (!this.isValidGrid(col, row)) return
    if (this.heroMoving) return

    // Tutorial: restrict taps to target cell only
    if (this.gameMode === GAME_MODE.TUTORIAL && this.tutorialState >= 0) {
      const target = TUT_TARGETS[this.tutorialState]
      if (!target) return
      if (col !== target.col || row !== target.row) return
      // Correct tap - process game logic then advance tutorial
      this.processTap(col, row)
      this.advanceTutorial()
      return
    }

    this.processTap(col, row)
  }

  processTap(col, row) {
    const barrier = this.getBarrierAt(col, row)
    if (barrier && barrier.type === BARRIER.BOX) {
      const dx = Math.abs(this.hero.col - col)
      const dy = Math.abs(this.hero.row - row)
      if ((dx === 1 && dy === 0) || (dx === 0 && dy === 1)) {
        this.pushBox(barrier)
      }
    } else if (!this.isBlocked(col, row)) {
      this.moveHeroTo(col, row)
    }
  }

  // ---- Hero movement ----
  moveHeroTo(targetCol, targetRow) {
    const obstacles = this.getObstacles()
    const path = AStar.findPath(this.hero.col, this.hero.row, targetCol, targetRow, obstacles)
    if (!path || path.length === 0) return
    this.heroPath = path
    this.heroMoving = true
    this.moveHeroStep()
  }

  moveHeroStep() {
    if (this.heroPath.length === 0) { this.heroMoving = false; return }

    const next = this.heroPath.shift()
    const dx = next.col - this.hero.col
    const dy = next.row - this.hero.row

    this.setFacing(dx, dy)

    const fromPos = this.gridToScreen(this.hero.col, this.hero.row)
    const toPos = this.gridToScreen(next.col, next.row)

    this.heroAnim = {
      fromX: fromPos.x, fromY: fromPos.y,
      toX: toPos.x, toY: toPos.y,
      duration: 200, elapsed: 0,
      onDone: () => {
        this.hero.col = next.col
        this.hero.row = next.row
        this.heroAnim = null
        this.checkPropCollect()
        if (!this.stopHeroCheck) this.checkHeroMonsterCollision()
        if (this.heroPath.length > 0 && this.state === 'playing') {
          this.moveHeroStep()
        } else {
          this.heroMoving = false
        }
      }
    }
  }

  pushBox(box) {
    const dirCol = box.col - this.hero.col
    const dirRow = box.row - this.hero.row

    const pushPath = []
    let cc = box.col + dirCol
    let cr = box.row + dirRow

    while (this.isValidGrid(cc, cr)) {
      if (this.getBarrierAt(cc, cr)) break
      pushPath.push({ col: cc, row: cr })
      cc += dirCol
      cr += dirRow
    }

    if (pushPath.length === 0) {
      // Obstacle right next to box — box shatters
      this.destroyBox(box)
      this.audio.playSound('hit')
      this.hero.col += dirCol
      this.hero.row += dirRow
      this.updateHeroAnim()
      this.setFacing(dirCol, dirRow)
      return
    }

    // Record monsters per cell — killed when box reaches each cell during animation
    const cellKills = pushPath.map(cell => ({
      col: cell.col, row: cell.row,
      monster: this.getMonsterAt(cell.col, cell.row) || null,
    }))

    // Move hero to box's old position
    this.hero.col += dirCol
    this.hero.row += dirRow

    // Box slides to the last empty cell and STOPS there (survives)
    const lastCell = pushPath[pushPath.length - 1]
    const fromPos = this.gridToScreen(box.col, box.row)
    const toPos = this.gridToScreen(lastCell.col, lastCell.row)

    this.grid[box.row][box.col] = 0
    this.grid[lastCell.row][lastCell.col] = BARRIER.BOX
    box.col = lastCell.col
    box.row = lastCell.row

    this.audio.playSound('push')

    this.anims.push({
      type: 'box', fromX: fromPos.x, fromY: fromPos.y,
      toX: toPos.x, toY: toPos.y,
      duration: pushPath.length * 160, elapsed: 0, barrier: box, destroy: false,
      _cellKills: cellKills, _killedUpTo: -1,
    })

    this.updateHeroAnim()
    this.setFacing(dirCol, dirRow)
  }

  updateHeroAnim() {
    const pos = this.gridToScreen(this.hero.col, this.hero.row)
    this.hero.animX = pos.x
    this.hero.animY = pos.y
  }

  destroyBox(box) {
    this.grid[box.row][box.col] = 0
    const idx = this.barriers.indexOf(box)
    if (idx >= 0) this.barriers.splice(idx, 1)
    this.boxDestroyCount++
    this.onPropTrigger()
  }

  killMonster(monster) {
    const idx = this.monsters.indexOf(monster)
    if (idx >= 0) {
      this.monsters.splice(idx, 1)
      const score = MONSTER_SCORE[monster.type] || 100
      this.score += score
      this.floatScores.push({ x: monster.animX, y: monster.animY, text: '+' + score, timer: 1000 })
      this.audio.playSound('kill')
    }
  }

  checkPropCollect() {
    for (let i = this.props.length - 1; i >= 0; i--) {
      const prop = this.props[i]
      if (prop.col === this.hero.col && prop.row === this.hero.row) {
        this.collectProp(prop)
        this.props.splice(i, 1)
      }
    }
  }

  collectProp(prop) {
    this.propGetCount++
    const score = PROP_SCORE[prop.type] || 500
    this.score += score
    const pos = this.gridToScreen(prop.col, prop.row)
    this.floatScores.push({ x: pos.x, y: pos.y, text: '+' + score, timer: 1000 })

    switch (prop.type) {
      case PROP.BOMB:
        this.audio.playSound('boom')
        for (let i = this.monsters.length - 1; i >= 0; i--) {
          const m = this.monsters[i]
          this.score += (MONSTER_SCORE[m.type] || 100)
          this.floatScores.push({ x: m.animX, y: m.animY, text: '+' + MONSTER_SCORE[m.type], timer: 1000 })
        }
        this.monsters = []
        break
      case PROP.CHERRY: this.audio.playSound('blood'); this.boxProp++; break
      case PROP.REDHEART: this.audio.playSound('blood'); this.blood++; break
      case PROP.SHOES: this.audio.playSound('yahoo'); break
    }
  }

  checkHeroMonsterCollision() {
    for (const monster of this.monsters) {
      if (monster.col === this.hero.col && monster.row === this.hero.row) {
        this.heroHurt(); break
      }
    }
  }

  heroHurt() {
    this.blood--
    this.audio.playSound('hurt')
    this.floatScores.push({ x: this.hero.animX, y: this.hero.animY, text: '-1', timer: 800, color: '#ff0000' })
    if (this.blood <= 0) {
      this.state = 'lose'
      this.audio.playSound('lose')
    } else {
      this.stopHeroCheck = true
      this._timeouts = this._timeouts || []
      this._timeouts.push(setTimeout(() => { this.stopHeroCheck = false }, 1500))
    }
  }

  addBoxAtRandom() {
    if (this.boxProp <= 0) return
    this.boxProp--
    for (let n = 0; n < 2; n++) {
      if (this.barriers.length > 50) break
      const emptyCells = []
      for (let r = 0; r < GRID_ROWS; r++)
        for (let c = 0; c < GRID_COLS; c++)
          if (this.grid[r][c] === 0 && !(c === this.hero.col && r === this.hero.row)) emptyCells.push({ col: c, row: r })
      if (emptyCells.length === 0) return
      const cell = emptyCells[Math.floor(Math.random() * emptyCells.length)]
      this.grid[cell.row][cell.col] = BARRIER.BOX
      this.barriers.push({ col: cell.col, row: cell.row, type: BARRIER.BOX, animX: 0, animY: 0 })
    }
  }

  // ---- Monster AI ----
  spawnMonsters(dt) {
    if (this.gameMode !== GAME_MODE.NORMAL) return
    if (this.monsterQueue.length === 0 || this.monsterSpawnIndex >= this.monsterQueue.length) return
    if (this.monsters.length >= MAX_MONSTERS) return
    this.monsterSpawnTimer += dt
    if (this.monsterSpawnTimer < this.monsterSpawnInterval) return
    this.monsterSpawnTimer = 0

    const type = this.monsterQueue[this.monsterSpawnIndex]
    const spawnCols = [0, 3, 7]
    const spawnCol = spawnCols[this.monsterSpawnIndex % 3]
    const spawnRow = GRID_ROWS - 1

    // Remove any barrier at spawn position (matching C++ GetBarrierByCross → OnDeleteBarrier)
    const barrier = this.getBarrierAt(spawnCol, spawnRow)
    if (barrier) this.destroyBox(barrier)
    if (this.grid[spawnRow][spawnCol] !== 0) return

    this.monsterSpawnIndex++
    this.grid[spawnRow][spawnCol] = 0
    const pos = this.gridToScreen(spawnCol, spawnRow)
    this.monsters.push({ col: spawnCol, row: spawnRow, type, animX: pos.x, animY: pos.y, fromX: pos.x, fromY: pos.y, toX: pos.x, toY: pos.y, moveElapsed: 1000, moveDuration: 1000, moveDelay: 500 })
  }

  moveMonster(monster) {
    const dist = Math.abs(monster.col - this.hero.col) + Math.abs(monster.row - this.hero.row)
    let newCol = null, newRow = null

    if (dist <= MONSTER_CHASE_DIST && dist > 0) {
      // Use A* to find path to hero (matching C++ GetCrossPath + followHero)
      const obstacles = this.getObstacles()
      obstacles.delete(monster.col + ',' + monster.row) // exclude self from obstacles
      const path = AStar.findPath(monster.col, monster.row, this.hero.col, this.hero.row, obstacles)
      if (path && path.length > 0) {
        newCol = path[0].col
        newRow = path[0].row
      }
    }

    // If no chase path, try random direction (cycling all 4 like C++)
    if (newCol === null) {
      const dirs = [[0,1],[0,-1],[1,0],[-1,0]]
      const startIdx = Math.floor(Math.random() * 4)
      for (let i = 0; i < 4; i++) {
        const [dc, dr] = dirs[(startIdx + i) % 4]
        const nc = monster.col + dc
        const nr = monster.row + dr
        if (this.isValidGrid(nc, nr) && !this.isBlocked(nc, nr) && !this.getMonsterAt(nc, nr)) {
          newCol = nc
          newRow = nr
          break
        }
      }
    }

    if (newCol !== null) {
      const fromPos = this.gridToScreen(monster.col, monster.row)
      monster.fromX = fromPos.x; monster.fromY = fromPos.y
      monster.col = newCol; monster.row = newRow
      const toPos = this.gridToScreen(newCol, newRow)
      monster.toX = toPos.x; monster.toY = toPos.y
      monster.moveElapsed = 0
    } else {
      // Surrounded — retry after short delay (matching C++ scheduleOnce 0.1f)
      monster.moveElapsed = monster.moveDuration - 100
    }
  }

  checkWinCondition() {
    if (this.state !== 'playing' || this.gameMode !== GAME_MODE.NORMAL) return
    const hasMonsters = this.monsters.length > 0
    const hasMoreMonsters = this.monsterSpawnIndex < this.monsterQueue.length
    if (!hasMonsters && !hasMoreMonsters && this.monsterQueue.length > 0) {
      this.state = 'win'
      this.audio.playSound('win')
      const stars = this.calculateStars()
      this.data.levelStars[this.levelNum] = Math.max(this.data.levelStars[this.levelNum] || 0, stars)
      if (this.levelNum >= this.data.unlockedLevel) this.data.unlockedLevel = this.levelNum + 1
      this.data.blood = this.blood
      this.data.currentLevel = this.levelNum + 1
      this.storage.saveGameData(this.data)
    }
  }

  calculateStars() {
    if (this.levelNum === 1) return 3
    return Math.min(this.propGetCount, 3)
  }

  handleResultTouch(x, y) {
    const centerX = DESIGN_W / 2
    if (this.state === 'win') {
      if (x >= centerX - 77 && x <= centerX + 77 && y >= 650 && y <= 730) {
        this.audio.playSound('hit'); this.onNavigate('game', { level: this.levelNum + 1 }); return
      }
      if (x >= centerX - 60 && x <= centerX + 60 && y >= 750 && y <= 830) {
        this.audio.playSound('hit'); this.onNavigate('level'); return
      }
    } else if (this.state === 'lose') {
      if (x >= centerX - 77 && x <= centerX + 77 && y >= 650 && y <= 730) {
        this.audio.playSound('hit'); this.initLevel(); this.audio.playSound('readygo'); return
      }
      if (x >= centerX - 60 && x <= centerX + 60 && y >= 750 && y <= 830) {
        this.audio.playSound('hit'); this.onNavigate('level'); return
      }
    }
  }

  // ---- Update ----
  update(dt) {
    if (this.state !== 'playing') return

    // Update scheduled callbacks
    this.updateSchedules(dt)

    // Hero walking animation (always runs, matching C++ RepeatForever)
    this.heroWalkTimer += dt
    if (this.heroWalkTimer >= 200) {
      this.heroWalkTimer -= 200
      this.heroWalkFrame = 1 - this.heroWalkFrame
    }

    if (this.heroAnim) {
      // Animate position
      this.heroAnim.elapsed += dt
      const t = Math.min(this.heroAnim.elapsed / this.heroAnim.duration, 1)
      const ease = t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t
      this.hero.animX = this.heroAnim.fromX + (this.heroAnim.toX - this.heroAnim.fromX) * ease
      this.hero.animY = this.heroAnim.fromY + (this.heroAnim.toY - this.heroAnim.fromY) * ease
      if (t >= 1 && this.heroAnim.onDone) this.heroAnim.onDone()
    }

    // Barrier animations
    for (let i = this.anims.length - 1; i >= 0; i--) {
      const anim = this.anims[i]
      anim.elapsed += dt
      const t = Math.min(anim.elapsed / anim.duration, 1)
      if (anim.barrier) {
        anim.barrier.animX = anim.fromX + (anim.toX - anim.fromX) * t
        anim.barrier.animY = anim.fromY + (anim.toY - anim.fromY) * t
      }
      if (anim._cellKills) {
        const cellCount = anim._cellKills.length
        const currentCell = Math.min(Math.floor(t * cellCount), cellCount - 1)
        if (currentCell > anim._killedUpTo) {
          for (let j = anim._killedUpTo + 1; j <= currentCell; j++) {
            if (anim._cellKills[j].monster) {
              this.killMonster(anim._cellKills[j].monster)
              anim._cellKills[j].monster = null
            }
          }
          anim._killedUpTo = currentCell
        }
      }
      if (t >= 1) {
        this.anims.splice(i, 1)
      }
    }

    // Tutorial final push animation completed → trigger win
    if (this._waitingForTutorialEnd && this.anims.length === 0) {
      this._waitingForTutorialEnd = false
      this.onTutorialWin()
    }

    // Monster movement (per-monster action-chaining, matching C++ MoveTo→CallFunc→MonsterMove)
    for (const m of this.monsters) {
      if (m.static) continue

      // Initial spawn delay (500ms)
      if (m.moveDelay > 0) {
        m.moveDelay -= dt
        continue
      }

      // Currently animating a move
      if (m.moveElapsed < m.moveDuration) {
        m.moveElapsed += dt
        const t = Math.min(m.moveElapsed / m.moveDuration, 1)
        m.animX = m.fromX + (m.toX - m.fromX) * t
        m.animY = m.fromY + (m.toY - m.fromY) * t
      } else {
        // Animation finished — decide next move immediately (no gap)
        this.moveMonster(m)
      }
    }

    // Hand pointer bob animation
    if (this.handVisible) {
      this.handBob += dt * 0.004
    }

    // Spawn monsters (timer-based) + per-frame collision check
    this.spawnMonsters(dt)
    if (!this.stopHeroCheck) this.checkHeroMonsterCollision()

    // Float scores
    for (let i = this.floatScores.length - 1; i >= 0; i--) {
      this.floatScores[i].timer -= dt
      this.floatScores[i].y -= dt * 0.05
      if (this.floatScores[i].timer <= 0) this.floatScores.splice(i, 1)
    }

    this.checkWinCondition()
  }

  blinkAlpha(obj) {
    if (!obj.blink) return 1
    obj.blinkTimer = (obj.blinkTimer || 0) + 16
    return Math.sin(obj.blinkTimer * 0.008) * 0.4 + 0.6
  }

  // ---- Render ----
  render(ctx) {
    // Background
    const bg = this.rm.images.game_bg
    if (bg && bg.width) ctx.drawImage(bg, 0, 0, DESIGN_W, DESIGN_H)
    else { ctx.fillStyle = '#d4edda'; ctx.fillRect(0, 0, DESIGN_W, DESIGN_H) }

    this.rm.drawFrame0(ctx, 'blue_background', 0, 0, DESIGN_W, 80)

    // Grid
    ctx.fillStyle = 'rgba(139, 119, 87, 0.15)'
    ctx.fillRect(GRID_ORIGIN_X, GRID_ORIGIN_Y, GRID_COLS * TILE_W, GRID_ROWS * TILE_H)
    ctx.strokeStyle = 'rgba(139, 119, 87, 0.3)'; ctx.lineWidth = 1
    for (let r = 0; r <= GRID_ROWS; r++) {
      ctx.beginPath(); ctx.moveTo(GRID_ORIGIN_X, GRID_ORIGIN_Y + r * TILE_H)
      ctx.lineTo(GRID_ORIGIN_X + GRID_COLS * TILE_W, GRID_ORIGIN_Y + r * TILE_H); ctx.stroke()
    }
    for (let c = 0; c <= GRID_COLS; c++) {
      ctx.beginPath(); ctx.moveTo(GRID_ORIGIN_X + c * TILE_W, GRID_ORIGIN_Y)
      ctx.lineTo(GRID_ORIGIN_X + c * TILE_W, GRID_ORIGIN_Y + GRID_ROWS * TILE_H); ctx.stroke()
    }

    // Hero path indicators
    if (this.heroPath.length > 0) {
      for (const p of this.heroPath) {
        const pos = this.gridToScreen(p.col, p.row)
        this.rm.drawFrame0(ctx, 'green', pos.x + TILE_W / 2 - 12, pos.y + TILE_H / 2 - 12, 24, 24)
      }
    }

    // Build sorted draw list (row-descending, matching C++ Z-order by row)
    const drawList = []

    for (const b of this.barriers) {
      let pos
      if (b.animX !== 0 || b.animY !== 0) pos = { x: b.animX, y: b.animY }
      else { pos = this.gridToScreen(b.col, b.row); b.animX = pos.x; b.animY = pos.y }
      drawList.push({ row: b.row, order: 0, draw: () => {
        ctx.globalAlpha = this.blinkAlpha(b)
        this.drawBarrier(ctx, b.type, pos.x, pos.y)
        ctx.globalAlpha = 1
      }})
    }

    for (const prop of this.props) {
      drawList.push({ row: prop.row, order: 1, draw: () => {
        ctx.globalAlpha = this.blinkAlpha(prop)
        this.drawProp(ctx, prop.type, prop.animX, prop.animY)
        ctx.globalAlpha = 1
      }})
    }

    for (const m of this.monsters) {
      drawList.push({ row: m.row, order: 2, draw: () => {
        this.drawMonster(ctx, m.type, m.animX, m.animY)
      }})
    }

    drawList.push({ row: this.hero.row, order: 3, draw: () => { this.drawHero(ctx) }})

    // Sort: highest row first (behind), lowest row last (front)
    drawList.sort((a, b) => b.row - a.row || a.order - b.order)
    for (const item of drawList) item.draw()

    // Hand pointer (tutorial)
    if (this.handVisible) {
      this.drawHand(ctx)
    }

    // Float scores
    for (const fs of this.floatScores) {
      ctx.globalAlpha = Math.min(fs.timer / 500, 1)
      ctx.fillStyle = fs.color || '#ffff00'
      ctx.font = 'bold 24px Arial'; ctx.textAlign = 'center'; ctx.textBaseline = 'middle'
      ctx.fillText(fs.text, fs.x + TILE_W / 2, fs.y + TILE_H / 2)
      ctx.globalAlpha = 1
    }

    this.drawHUD(ctx)
    if (this.state === 'win' || this.state === 'lose') this.drawResult(ctx)
    this.rm.drawFrame0(ctx, 'blue_background', 0, DESIGN_H - 80, DESIGN_W, 80)
  }

  drawBarrier(ctx, type, x, y) {
    switch (type) {
      case BARRIER.BOX:      this.rm.drawFrame0(ctx, 'box', x + 4, y - 10, TILE_W - 8, TILE_H); break
      case BARRIER.STONE:    this.rm.drawFrame1(ctx, 'stone', x + 4, y - 10, TILE_W - 8, TILE_H); break
      case BARRIER.SMALLTREE:this.rm.drawFrame1(ctx, 'tree_small', x + 20, y - 15, 40, TILE_H + 10); break
      case BARRIER.BIGTREE:  this.rm.drawFrame1(ctx, 'tree_big', x + 2, y - 30, TILE_W - 4, TILE_H + 30); break
      case BARRIER.FLOWER:   this.rm.drawFrame0(ctx, 'flower', x + 12, y + 10, 35, 30); break
    }
  }

  drawProp(ctx, type, x, y) {
    const size = 50, ox = (TILE_W - size) / 2, oy = (TILE_H - size) / 2
    switch (type) {
      case PROP.BOMB:     this.rm.drawFrame0(ctx, 'bomb', x + ox, y + oy - 5, size, size); break
      case PROP.CHERRY:   this.rm.drawFrame0(ctx, 'cherry', x + ox, y + oy - 5, size, size); break
      case PROP.REDHEART: this.rm.drawFrame0(ctx, 'redheart', x + ox, y + oy - 5, size, size); break
      case PROP.SHOES:    this.rm.drawFrame1(ctx, 'shoes', x + ox, y + oy - 5, size, size); break
    }
  }

  drawMonster(ctx, type, x, y) {
    let frame
    switch (type) {
      case MONSTER.BUG: frame = 'monster_bug'; break
      case MONSTER.RED: frame = 'monster_red'; break
      case MONSTER.PURPLE: frame = 'monster_purple'; break
      default: frame = 'monster_bug'
    }
    this.rm.drawFrame0(ctx, frame, x + (TILE_W - 60) / 2, y + (TILE_H - 60) / 2 - 5, 60, 60)
  }

  drawHero(ctx) {
    const dir = this.heroFacing || 'left'
    const frames = HERO_FRAMES[dir] || HERO_FRAMES.left
    // Always alternate frames, matching C++ RepeatForever animation
    const frame = frames[this.heroWalkFrame]

    const hx = this.hero.animX || this.gridToScreen(this.hero.col, this.hero.row).x
    const hy = this.hero.animY || this.gridToScreen(this.hero.col, this.hero.row).y
    this.rm.drawFrame0(ctx, frame, hx - 10, hy - 35, TILE_W + 20, TILE_H + 30)
  }

  drawHand(ctx) {
    const pos = this.gridToScreen(this.handCol, this.handRow)
    const bobY = Math.sin(this.handBob) * 8
    // Center the hand over the grid cell, finger tip pointing at cell bottom (matching C++ hand position)
    const handW = 60, handH = 76
    const cx = pos.x + TILE_W / 2 - handW / 2 + 10 // slight right offset so finger aligns with cell center
    const cy = pos.y + TILE_H * 1.5 - handH + bobY
    this.rm.drawFrame0(ctx, 'hander', cx, cy, handW, handH)
  }

  drawHUD(ctx) {
    ctx.fillStyle = '#ffffff'; ctx.font = 'bold 20px Arial'; ctx.textAlign = 'left'; ctx.textBaseline = 'top'
    ctx.fillText('Lv.' + this.levelNum, 15, 10)

    this.rm.drawFrame0(ctx, 'score', 100, 10, 80, 22)
    ctx.fillStyle = '#ffffff'; ctx.font = '16px Arial'; ctx.fillText(String(this.score), 170, 12)

    for (let i = 0; i < this.blood; i++) this.rm.drawFrame0(ctx, 'blood', 260 + i * 35, 5, 30, 25)

    if (this.boxProp > 0) {
      ctx.fillStyle = '#ffffff'; ctx.font = 'bold 18px Arial'; ctx.textAlign = 'center'
      ctx.fillText('Box x' + this.boxProp, DESIGN_W - 65, 15)
    }

    this.rm.drawFrame0(ctx, 'pause_normal', DESIGN_W - 90, 40, 60, 60)

    ctx.fillStyle = '#ffffff'; ctx.font = '16px Arial'; ctx.textAlign = 'center'
    const monsterLeft = this.monsters.length + (this.monsterQueue.length - this.monsterSpawnIndex)
    ctx.fillText('Monsters: ' + monsterLeft, DESIGN_W / 2, DESIGN_H - 30)

    if (this.boxProp > 0) {
      ctx.fillStyle = '#ff9800'; ctx.fillRect(DESIGN_W - 110, DESIGN_H - 70, 90, 40)
      ctx.fillStyle = '#ffffff'; ctx.font = 'bold 16px Arial'; ctx.textAlign = 'center'
      ctx.fillText('+ Box', DESIGN_W - 65, DESIGN_H - 50)
    }
  }

  drawResult(ctx) {
    ctx.fillStyle = 'rgba(0, 0, 0, 0.6)'; ctx.fillRect(0, 0, DESIGN_W, DESIGN_H)
    const centerX = DESIGN_W / 2

    if (this.state === 'win') {
      this.rm.drawFrame1(ctx, 'result', centerX - 142, 200, 285, 149)
      const stars = this.calculateStars()
      for (let i = 0; i < 3; i++) {
        if (i < stars) { this.rm.drawFrame1(ctx, 'star_result', centerX - 85 + i * 60, 370, 55, 55) }
        else { ctx.globalAlpha = 0.3; this.rm.drawFrame1(ctx, 'star_result', centerX - 85 + i * 60, 370, 55, 55); ctx.globalAlpha = 1 }
      }
      ctx.fillStyle = '#ffffff'; ctx.font = 'bold 28px Arial'; ctx.textAlign = 'center'
      ctx.fillText('Score: ' + this.score, centerX, 470)
      this.rm.drawFrame0(ctx, 'next_normal', centerX - 77, 650, 154, 66)
      this.rm.drawFrame0(ctx, 'menu_normal', centerX - 60, 750, 120, 66)
    } else {
      ctx.fillStyle = '#ff4444'; ctx.font = 'bold 48px Arial'; ctx.textAlign = 'center'; ctx.fillText('GAME OVER', centerX, 300)
      ctx.fillStyle = '#ffffff'; ctx.font = '28px Arial'; ctx.fillText('Score: ' + this.score, centerX, 400)
      this.rm.drawFrame1(ctx, 'restart_normal', centerX - 77, 650, 154, 66)
      this.rm.drawFrame0(ctx, 'menu_normal', centerX - 60, 750, 120, 66)
    }
  }
}

module.exports = { GameScene }

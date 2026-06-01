// Level selection scene
const { DESIGN_W, DESIGN_H, LEVELS } = require('./config')

class LevelScene {
  constructor(rm, audio, storage, onNavigate) {
    this.rm = rm
    this.audio = audio
    this.storage = storage
    this.onNavigate = onNavigate
    this.page = 1
    this.perPage = 20
    this.totalPages = 2
    this.swipeThreshold = 50
  }

  show() {
    this.data = this.storage.loadGameData()
  }

  hide() {}

  onTouchStart(x, y) {
    this._touchStartX = x
    this._touchStartY = y
  }

  onTouchEnd(x, y) {
    const dx = x - this._touchStartX
    const dy = y - this._touchStartY

    // Swipe detection: horizontal swipe to change pages
    if (Math.abs(dx) > this.swipeThreshold && Math.abs(dx) > Math.abs(dy)) {
      if (dx < 0 && this.page < this.totalPages) {
        this.page++
        this.audio.playSound('box')
      } else if (dx > 0 && this.page > 1) {
        this.page--
        this.audio.playSound('box')
      }
      return
    }

    // Level buttons - 4 columns x 5 rows
    const startX = 60
    const startY = 150
    const spacing = 140
    const btnSize = 100

    for (let i = 0; i < this.perPage; i++) {
      const levelNum = (this.page - 1) * this.perPage + i + 1
      if (levelNum > 31) break // max 30 levels + 1 infinite

      const col = i % 4
      const row = Math.floor(i / 4)
      const bx = startX + col * spacing
      const by = startY + row * spacing

      if (x >= bx && x <= bx + btnSize && y >= by && y <= by + btnSize) {
        if (levelNum <= this.data.unlockedLevel) {
          this.audio.playSound('hit')
          this.onNavigate('game', { level: levelNum })
        }
        return
      }
    }
  }

  update(dt) {}

  render(ctx) {
    this.data = this.storage.loadGameData()

    // Background
    ctx.fillStyle = '#e8f5e9'
    ctx.fillRect(0, 0, DESIGN_W, DESIGN_H)

    // Top bar
    this.rm.drawFrame0(ctx, 'blue_background', 0, 0, DESIGN_W, 100)

    // Title
    ctx.fillStyle = '#ffffff'
    ctx.font = 'bold 36px Arial'
    ctx.textAlign = 'center'
    ctx.textBaseline = 'middle'
    ctx.fillText('选择关卡', DESIGN_W / 2, 50)

    // Level buttons
    const startX = 60
    const startY = 150
    const spacing = 140
    const btnSize = 100

    for (let i = 0; i < this.perPage; i++) {
      const levelNum = (this.page - 1) * this.perPage + i + 1
      if (levelNum > 31) break

      const col = i % 4
      const row = Math.floor(i / 4)
      const bx = startX + col * spacing
      const by = startY + row * spacing

      if (levelNum <= this.data.unlockedLevel) {
        // Unlocked level
        this.rm.drawFrame0(ctx, 'level_normal', bx, by, btnSize, btnSize)

        // Level number
        ctx.fillStyle = '#333333'
        ctx.font = 'bold 32px Arial'
        ctx.textAlign = 'center'
        ctx.textBaseline = 'middle'
        ctx.fillText(String(levelNum), bx + btnSize / 2, by + btnSize / 2)

        // Stars
        const stars = this.data.levelStars[levelNum] || 0
        for (let s = 0; s < stars; s++) {
          this.rm.drawFrame0(ctx, 'level_star', bx + 15 + s * 25, by + btnSize - 20, 20, 20)
        }
      } else {
        // Locked level
        this.rm.drawFrame0(ctx, 'level_lock', bx, by, btnSize, btnSize)
      }
    }

    // Page indicator
    ctx.fillStyle = '#666666'
    ctx.font = '24px Arial'
    ctx.textAlign = 'center'
    ctx.fillText(`${this.page} / ${this.totalPages}`, DESIGN_W / 2, DESIGN_H - 40)
  }
}

module.exports = { LevelScene }

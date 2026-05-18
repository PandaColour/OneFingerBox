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
  }

  show() {
    this.data = this.storage.loadGameData()
  }

  hide() {}

  onTouchStart(x, y) {}

  onTouchEnd(x, y) {
    // Back button (top-left)
    if (x >= 20 && x <= 176 && y >= 20 && y <= 105) {
      this.audio.playSound('hit')
      this.onNavigate('main')
      return
    }

    // Page arrows
    if (this.page > 1 && x >= 20 && x <= 78 && y >= DESIGN_H / 2 - 51 && y <= DESIGN_H / 2 + 51) {
      this.page--
      this.audio.playSound('box')
      return
    }
    if (this.page < this.totalPages && x >= DESIGN_W - 78 && x <= DESIGN_W - 20 && y >= DESIGN_H / 2 - 51 && y <= DESIGN_H / 2 + 51) {
      this.page++
      this.audio.playSound('box')
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

    // Back button
    this.rm.drawFrame0(ctx, 'exit_normal', 20, 20, 120, 66)

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

    // Page arrows
    if (this.page > 1) {
      this.rm.drawFrame0(ctx, 'level_arrow_normal', 20, DESIGN_H / 2 - 51, 46, 81)
    }
    if (this.page < this.totalPages) {
      this.rm.drawFrame0(ctx, 'level_arrow_normal', DESIGN_W - 66, DESIGN_H / 2 - 51, 46, 81)
    }

    // Page indicator
    ctx.fillStyle = '#666666'
    ctx.font = '24px Arial'
    ctx.textAlign = 'center'
    ctx.fillText(`${this.page} / ${this.totalPages}`, DESIGN_W / 2, DESIGN_H - 40)
  }
}

module.exports = { LevelScene }

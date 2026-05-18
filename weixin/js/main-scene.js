// Main menu scene
const { DESIGN_W, DESIGN_H } = require('./config')

class MainScene {
  constructor(rm, audio, onNavigate) {
    this.rm = rm
    this.audio = audio
    this.onNavigate = onNavigate
    this.touchStart = null
  }

  show() {
    this.audio.playBGM()
  }

  hide() {}

  onTouchStart(x, y) {
    this.touchStart = { x, y }
  }

  onTouchEnd(x, y) {
    // Start button area (design coords: centered, y ~550-680)
    const scaleX = DESIGN_W
    const btnX = DESIGN_W / 2 - 121
    const btnY = 550
    const btnW = 243
    const btnH = 133

    if (x >= btnX && x <= btnX + btnW && y >= btnY && y <= btnY + btnH) {
      this.audio.playSound('hit')
      this.onNavigate('level')
      return
    }

    // Settings button (top-right area)
    if (x >= DESIGN_W - 120 && x <= DESIGN_W - 20 && y >= 30 && y <= 130) {
      this.audio.playSound('hit')
      this.onNavigate('setting')
      return
    }
  }

  update(dt) {}

  render(ctx) {
    // Background
    const bg = this.rm.images.main_bg
    if (bg && bg.width) {
      ctx.drawImage(bg, 0, 0, DESIGN_W, DESIGN_H)
    } else {
      ctx.fillStyle = '#4a8c3f'
      ctx.fillRect(0, 0, DESIGN_W, DESIGN_H)
    }

    // Blue background bar at top
    this.rm.drawFrame0(ctx, 'blue_background', 0, 0, DESIGN_W, 188)

    // Logo
    this.rm.drawFrame0(ctx, 'main_logo', (DESIGN_W - 427) / 2, 20, 427, 225)

    // Start button
    this.rm.drawFrame0(ctx, 'main_startnormal', DESIGN_W / 2 - 121, 550, 243, 133)

    // Settings button
    this.rm.drawFrame1(ctx, 'setting_normal', DESIGN_W - 120, 30, 100, 100)
  }
}

module.exports = { MainScene }

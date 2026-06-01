// Setting scene
const { DESIGN_W, DESIGN_H } = require('./config')

class SettingScene {
  constructor(rm, audio, onNavigate) {
    this.rm = rm
    this.audio = audio
    this.onNavigate = onNavigate
    this.fromGame = false
    this.level = 1
  }

  show(params) {
    this.fromGame = !!(params && params.fromGame)
    this.level = (params && params.level) || 1
  }

  hide() {}

  onTouchStart(x, y) {}

  onTouchEnd(x, y) {
    const centerX = DESIGN_W / 2

    // Music toggle
    if (x >= centerX - 85 && x <= centerX + 85 && y >= 300 && y <= 400) {
      const enabled = this.audio.toggleMusic()
      if (enabled) this.audio.playBGM()
    }

    // Sound toggle
    if (x >= centerX - 85 && x <= centerX + 85 && y >= 430 && y <= 530) {
      this.audio.toggleSound()
      this.audio.playSound('box')
    }

    // Back button
    if (x >= centerX - 73 && x <= centerX + 73 && y >= 600 && y <= 682) {
      this.audio.playSound('hit')
      if (this.fromGame) {
        this.onNavigate('game', { level: this.level })
      } else {
        this.onNavigate('main')
      }
    }
  }

  update(dt) {}

  render(ctx) {
    // Background
    ctx.fillStyle = '#f5f5dc'
    ctx.fillRect(0, 0, DESIGN_W, DESIGN_H)

    // Top bar
    this.rm.drawFrame0(ctx, 'blue_background', 0, 0, DESIGN_W, 100)

    // Title
    ctx.fillStyle = '#ffffff'
    ctx.font = 'bold 36px Arial'
    ctx.textAlign = 'center'
    ctx.textBaseline = 'middle'
    ctx.fillText('Settings', DESIGN_W / 2, 50)

    const centerX = DESIGN_W / 2

    // Music toggle
    if (this.audio.musicEnabled) {
      this.rm.drawFrame0(ctx, 'music_open', centerX - 85, 300, 170, 80)
    } else {
      this.rm.drawFrame0(ctx, 'music_close', centerX - 85, 300, 170, 80)
    }
    // Sound toggle
    if (this.audio.soundEnabled) {
      this.rm.drawFrame1(ctx, 'sound_open', centerX - 85, 430, 170, 80)
    } else {
      this.rm.drawFrame1(ctx, 'sound_close', centerX - 85, 430, 170, 80)
    }
    // Back button
    this.rm.drawFrame1(ctx, 'restart_normal', centerX - 77, 600, 154, 66)
  }
}

module.exports = { SettingScene }

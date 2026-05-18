// Scene manager - handles scene transitions
const { MainScene } = require('./main-scene')
const { LevelScene } = require('./level-scene')
const { GameScene } = require('./game-scene')
const { SettingScene } = require('./setting-scene')

class SceneManager {
  constructor(canvas, ctx, rm, audio, storage) {
    this.canvas = canvas
    this.ctx = ctx
    this.rm = rm
    this.audio = audio
    this.storage = storage

    this.scenes = {}
    this.currentScene = null
    this.currentSceneName = ''

    // Calculate scaling
    const sysInfo = wx.getSystemInfoSync()
    this.screenW = sysInfo.windowWidth
    this.screenH = sysInfo.windowHeight
    this.scale = Math.min(this.screenW / 640, this.screenH / 960)
    this.offsetX = (this.screenW - 640 * this.scale) / 2
    this.offsetY = (this.screenH - 960 * this.scale) / 2

    // Create scenes
    const onNavigate = (scene, params) => this.showScene(scene, params)
    this.scenes = {
      main: new MainScene(rm, audio, onNavigate),
      level: new LevelScene(rm, audio, storage, onNavigate),
      game: new GameScene(rm, audio, storage, onNavigate),
      setting: new SettingScene(rm, audio, onNavigate),
    }
  }

  showScene(name, params) {
    if (this.currentScene) {
      this.currentScene.hide()
    }
    this.currentSceneName = name
    this.currentScene = this.scenes[name]
    if (this.currentScene) {
      this.currentScene.show(params || {})
    }
  }

  update(dt) {
    if (this.currentScene) {
      this.currentScene.update(dt)
    }
  }

  render() {
    if (!this.currentScene) return

    const ctx = this.ctx
    ctx.clearRect(0, 0, this.canvas.width, this.canvas.height)

    ctx.save()
    ctx.translate(this.offsetX, this.offsetY)
    ctx.scale(this.scale, this.scale)

    this.currentScene.render(ctx)

    ctx.restore()
  }

  // Convert screen touch to design coordinates and forward to scene
  handleTouch(type, screenX, screenY) {
    if (!this.currentScene) return

    const designX = (screenX - this.offsetX) / this.scale
    const designY = (screenY - this.offsetY) / this.scale

    if (designX < 0 || designX > 640 || designY < 0 || designY > 960) return

    if (type === 'start') {
      this.currentScene.onTouchStart(designX, designY)
    } else {
      this.currentScene.onTouchEnd(designX, designY)
    }
  }
}

module.exports = { SceneManager }

// Audio manager - handles music and sound effects
const { AUDIO_FILES } = require('./config')

class AudioManager {
  constructor() {
    this.musicEnabled = true
    this.soundEnabled = true
    this.bgm = null
    this.sounds = {}
  }

  init() {
    // Load settings from storage
    try {
      const data = wx.getStorageSync('ofb_settings')
      if (data) {
        const settings = JSON.parse(data)
        this.musicEnabled = settings.musicEnabled !== false
        this.soundEnabled = settings.soundEnabled !== false
      }
    } catch (e) {
      // ignore
    }
  }

  saveSettings() {
    try {
      wx.setStorageSync('ofb_settings', JSON.stringify({
        musicEnabled: this.musicEnabled,
        soundEnabled: this.soundEnabled,
      }))
    } catch (e) {
      // ignore
    }
  }

  playBGM() {
    if (!this.musicEnabled) return
    this.stopBGM()
    this.bgm = wx.createInnerAudioContext()
    this.bgm.src = AUDIO_FILES.backmusic
    this.bgm.loop = true
    this.bgm.volume = 0.5
    try { this.bgm.play() } catch(e) {}
  }

  stopBGM() {
    if (this.bgm) {
      this.bgm.stop()
      this.bgm.destroy()
      this.bgm = null
    }
  }

  playSound(name) {
    if (!this.soundEnabled) return
    const src = AUDIO_FILES[name]
    if (!src) return

    const audio = wx.createInnerAudioContext()
    audio.src = src
    audio.volume = 0.8
    audio.onEnded(() => { audio.destroy() })
    audio.onError(() => { audio.destroy() })
    try { audio.play() } catch(e) {}
  }

  toggleMusic() {
    this.musicEnabled = !this.musicEnabled
    if (!this.musicEnabled) {
      this.stopBGM()
    }
    this.saveSettings()
    return this.musicEnabled
  }

  toggleSound() {
    this.soundEnabled = !this.soundEnabled
    this.saveSettings()
    return this.soundEnabled
  }
}

module.exports = { AudioManager }

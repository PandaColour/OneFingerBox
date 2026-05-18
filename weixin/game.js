// OneFingerBox - WeChat Mini Game Entry Point
const { ResourceManager } = require('./js/resource-manager')
const { AudioManager } = require('./js/audio-manager')
const { SceneManager } = require('./js/scene-manager')
const { loadGameData, saveGameData } = require('./js/storage')

// Create main display canvas
const canvas = wx.createCanvas()
const ctx = canvas.getContext('2d')

// Loading screen
function showLoading(progress) {
  ctx.fillStyle = '#2e7d32'
  ctx.fillRect(0, 0, canvas.width, canvas.height)

  ctx.fillStyle = '#ffffff'
  ctx.font = 'bold 28px Arial'
  ctx.textAlign = 'center'
  ctx.textBaseline = 'middle'
  ctx.fillText('OneFingerBox', canvas.width / 2, canvas.height / 2 - 50)

  ctx.font = '18px Arial'
  ctx.fillText('Loading... ' + progress + '%', canvas.width / 2, canvas.height / 2 + 10)

  // Progress bar
  const barW = canvas.width * 0.6
  const barH = 10
  const barX = (canvas.width - barW) / 2
  const barY = canvas.height / 2 + 40
  ctx.fillStyle = '#ffffff'
  ctx.fillRect(barX, barY, barW, barH)
  ctx.fillStyle = '#4caf50'
  ctx.fillRect(barX, barY, barW * progress / 100, barH)
}

showLoading(0)

// Initialize managers
const rm = new ResourceManager()
const audio = new AudioManager()
audio.init()

const storage = { loadGameData, saveGameData }

// Load all resources
rm.loadAll().then(() => {
  showLoading(100)

  // Create scene manager
  const sm = new SceneManager(canvas, ctx, rm, audio, storage)

  // Start at main menu
  sm.showScene('main')

  // Game loop
  let lastTime = Date.now()

  function gameLoop() {
    const now = Date.now()
    const dt = now - lastTime
    lastTime = now

    sm.update(dt)
    sm.render()

    requestAnimationFrame(gameLoop)
  }

  requestAnimationFrame(gameLoop)

  // Touch events
  wx.onTouchStart((res) => {
    if (res.touches.length > 0) {
      const touch = res.touches[0]
      sm.handleTouch('start', touch.clientX, touch.clientY)
    }
  })

  wx.onTouchEnd((res) => {
    if (res.changedTouches.length > 0) {
      const touch = res.changedTouches[0]
      sm.handleTouch('end', touch.clientX, touch.clientY)
    }
  })
}).catch((err) => {
  console.error('Failed to load resources:', err)
  ctx.fillStyle = '#ff0000'
  ctx.font = '20px Arial'
  ctx.textAlign = 'center'
  ctx.fillText('Load failed: ' + err.message, canvas.width / 2, canvas.height / 2)
})

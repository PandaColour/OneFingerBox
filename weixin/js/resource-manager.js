// Resource manager - loads images and provides sprite frame drawing
const { FRAMES_0, FRAMES_1, IMAGE_FILES } = require('./config')

class ResourceManager {
  constructor() {
    this.images = {}
    this.loaded = false
    this.loadCount = 0
    this.totalCount = Object.keys(IMAGE_FILES).length
  }

  loadAll() {
    return new Promise((resolve) => {
      if (this.totalCount === 0) {
        this.loaded = true
        resolve()
        return
      }

      for (const [key, src] of Object.entries(IMAGE_FILES)) {
        const img = wx.createImage()
        img.onload = () => {
          this.loadCount++
          if (this.loadCount >= this.totalCount) {
            this.loaded = true
            resolve()
          }
        }
        img.onerror = () => {
          console.warn('Failed to load image:', src)
          this.loadCount++
          if (this.loadCount >= this.totalCount) {
            this.loaded = true
            resolve()
          }
        }
        img.src = src
        this.images[key] = img
      }
    })
  }

  // Draw a sprite frame from sheet0
  drawFrame0(ctx, name, dx, dy, dw, dh) {
    const frame = FRAMES_0[name]
    if (!frame || !this.images.sheet0) return
    ctx.drawImage(this.images.sheet0, frame.x, frame.y, frame.w, frame.h, dx, dy, dw || frame.w, dh || frame.h)
  }

  // Draw a sprite frame from sheet1
  drawFrame1(ctx, name, dx, dy, dw, dh) {
    const frame = FRAMES_1[name]
    if (!frame || !this.images.sheet1) return
    ctx.drawImage(this.images.sheet1, frame.x, frame.y, frame.w, frame.h, dx, dy, dw || frame.w, dh || frame.h)
  }

  // Draw image directly (for backgrounds)
  drawImage(ctx, key, dx, dy, dw, dh) {
    const img = this.images[key]
    if (!img) return
    if (dw !== undefined) {
      ctx.drawImage(img, dx, dy, dw, dh)
    } else {
      ctx.drawImage(img, dx, dy)
    }
  }
}

module.exports = { ResourceManager }

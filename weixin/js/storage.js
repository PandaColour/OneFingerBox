// Storage - save/load game state

const STORAGE_KEY = 'ofb_game_data'

const defaultData = {
  currentLevel: 1,
  blood: 3,
  boxProp: 0,
  levelStars: {},  // { "1": 3, "2": 2, ... }
  unlockedLevel: 1,
}

function loadGameData() {
  try {
    const raw = wx.getStorageSync(STORAGE_KEY)
    if (raw) {
      const data = JSON.parse(raw)
      return Object.assign({}, defaultData, data)
    }
  } catch (e) {
    // ignore
  }
  return Object.assign({}, defaultData)
}

function saveGameData(data) {
  try {
    wx.setStorageSync(STORAGE_KEY, JSON.stringify(data))
  } catch (e) {
    // ignore
  }
}

function resetGameData() {
  saveGameData(Object.assign({}, defaultData))
  return Object.assign({}, defaultData)
}

module.exports = { loadGameData, saveGameData, resetGameData }

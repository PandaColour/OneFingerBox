// A* pathfinding algorithm
// Grid coords: col (0-7), row (0-9), row 0 = bottom

class AStar {
  // Find path from start to end, avoiding obstacles
  // obstacles: Set of "col,row" strings
  static findPath(startCol, startRow, endCol, endRow, obstacles) {
    if (startCol === endCol && startRow === endRow) return []

    const key = (c, r) => c + ',' + r
    const openSet = []
    const closedSet = new Set()
    const gScore = {}
    const fScore = {}
    const cameFrom = {}

    const startKey = key(startCol, startRow)
    gScore[startKey] = 0
    fScore[startKey] = AStar.heuristic(startCol, startRow, endCol, endRow)
    openSet.push({ col: startCol, row: startRow, f: fScore[startKey] })

    const dirs = [[0, 1], [0, -1], [1, 0], [-1, 0]]

    while (openSet.length > 0) {
      // Find node with lowest fScore
      openSet.sort((a, b) => a.f - b.f)
      const current = openSet.shift()
      const curKey = key(current.col, current.row)

      if (current.col === endCol && current.row === endRow) {
        // Reconstruct path
        const path = []
        let k = curKey
        while (cameFrom[k]) {
          const [c, r] = cameFrom[k].split(',').map(Number)
          path.unshift({ col: current.col, row: current.row })
          // Actually, store the move direction
          k = cameFrom[k]
        }
        // Rebuild path properly
        return AStar.reconstructPath(cameFrom, curKey)
      }

      closedSet.add(curKey)

      for (const [dc, dr] of dirs) {
        const nc = current.col + dc
        const nr = current.row + dr
        const nk = key(nc, nr)

        // Bounds check
        if (nc < 0 || nc >= 8 || nr < 0 || nr >= 10) continue
        // Obstacle check
        if (obstacles.has(nk)) continue
        // Already visited
        if (closedSet.has(nk)) continue

        const tentativeG = gScore[curKey] + 1

        if (gScore[nk] === undefined || tentativeG < gScore[nk]) {
          cameFrom[nk] = curKey
          gScore[nk] = tentativeG
          fScore[nk] = tentativeG + AStar.heuristic(nc, nr, endCol, endRow)

          if (!openSet.find(n => n.col === nc && n.row === nr)) {
            openSet.push({ col: nc, row: nr, f: fScore[nk] })
          }
        }
      }
    }

    return null // No path found
  }

  static reconstructPath(cameFrom, endKey) {
    const path = []
    let k = endKey
    while (cameFrom[k]) {
      const [c, r] = k.split(',').map(Number)
      path.unshift({ col: c, row: r })
      k = cameFrom[k]
    }
    return path
  }

  static heuristic(c1, r1, c2, r2) {
    return Math.abs(c1 - c2) + Math.abs(r1 - r2)
  }
}

module.exports = { AStar }

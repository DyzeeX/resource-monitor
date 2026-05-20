import { app, BrowserWindow } from 'electron'
import { spawn, ChildProcess } from 'child_process'
import path from 'path'

let mainWindow: BrowserWindow | null = null
let backendProcess: ChildProcess | null = null

const BACKEND_PATH = path.join(__dirname, '../../Backend/build/bin/resource-monitor')

function startBackend() {
  console.log('Starting backend:', BACKEND_PATH)

  backendProcess = spawn(BACKEND_PATH, [], { stdio: 'pipe' })

  backendProcess.stdout?.on('data', (data) => {
    console.log('[C++]:', data.toString())
  })

  backendProcess.stderr?.on('data', (data) => {
    console.error('[C++ err]:', data.toString())
  })

  backendProcess.on('close', (code) => {
    console.log(`C++ ended with code ${code}`)
    if (code !== 0) {
      setTimeout(startBackend, 3000)
    }
  })
}

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 800,
    backgroundColor: '#0f0f0f',
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true
    }
  })

  const isDev = process.env.NODE_ENV === 'development'

  if (isDev) {
    mainWindow.loadURL('http://localhost:5173')
    mainWindow.webContents.openDevTools()
  } else {
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'))
  }

  mainWindow.on('closed', () => {
    mainWindow = null
  })
}

app.whenReady().then(() => {
  startBackend()
  createWindow()
})

app.on('before-quit', () => {
  if (backendProcess) backendProcess.kill()
})

app.on('window-all-closed', () => {
  app.quit()
})
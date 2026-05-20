"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const electron_1 = require("electron");
const child_process_1 = require("child_process");
const path_1 = __importDefault(require("path"));
let mainWindow = null;
let backendProcess = null;
const BACKEND_PATH = path_1.default.join(__dirname, '../../Backend/build/bin/resource-monitor');
function startBackend() {
    console.log('Starting backend:', BACKEND_PATH);
    backendProcess = (0, child_process_1.spawn)(BACKEND_PATH, [], { stdio: 'pipe' });
    backendProcess.stdout?.on('data', (data) => {
        console.log('[C++]:', data.toString());
    });
    backendProcess.stderr?.on('data', (data) => {
        console.error('[C++ err]:', data.toString());
    });
    backendProcess.on('close', (code) => {
        console.log(`C++ ended with code ${code}`);
        if (code !== 0) {
            setTimeout(startBackend, 3000);
        }
    });
}
function createWindow() {
    mainWindow = new electron_1.BrowserWindow({
        width: 1280,
        height: 800,
        backgroundColor: '#0f0f0f',
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true
        }
    });
    const isDev = process.env.NODE_ENV === 'development';
    if (isDev) {
        mainWindow.loadURL('http://localhost:5173');
        mainWindow.webContents.openDevTools();
    }
    else {
        mainWindow.loadFile(path_1.default.join(__dirname, '../dist/index.html'));
    }
    mainWindow.on('closed', () => {
        mainWindow = null;
    });
}
electron_1.app.whenReady().then(() => {
    startBackend();
    createWindow();
});
electron_1.app.on('before-quit', () => {
    if (backendProcess)
        backendProcess.kill();
});
electron_1.app.on('window-all-closed', () => {
    electron_1.app.quit();
});

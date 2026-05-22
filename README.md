# Resource Monitor
Programm is aimed at monitoring resources of PC for Linux.

## Backend/  
C++ code is compiled with CMake. WebSocket selected as IPC. For JSON format files used [nlohmann/json](https://github.com/nlohmann/json). Hetwork part realised with [Crow](https://github.com/CrowCpp/Crow).  

### `main.cpp`  
Main C++ file, send JSON package through WebSocket to 2006 port

### `Components/`
*Component class* - base class with **Update()** method. Suggested, that it will update every component information.  

*CPUComponent class* - reads CPU information from `/proc/stat` file and calculate CPU usage value.  

*RAMComponent class* - reads RAM information from `/proc/meminfo` file and using **struct RAMStats** method **usage_percent()** calculate RAM usage.   

*ProcessComponent class* - stores every process information from `/proc/<PID>/status` in **std::vector<ProcessFileStat>** (note: in /proc directory stored all currently running processes ID's (PID), our goal is find all numeric names of directories). Also finds name of user (by process uid), starting the process using system call **getpwuid_r()**.  

*DiskComponent class* - reads Disk information from `/proc/diskstats` file. Get read/write bytes per second **GetReadBytesPerSec()/GetWriteBytesPerSec()** methods and disk occupancy information **GetIOUtilPercent()** (note: reads only 'sda' disk).    

### `Core/`
*ResourceMonitor class* - storage for all components, in **Update()** method delegated all components **Upadte()** methods. Access to every component only through **Get()**.  

*JSONMaanger class* - method **Convert()** converts all collected data in JSON-file and then **ConvertToString()** converts it in string. Packet example:
```json
{
  "cpu": {
    "usage": 23.47
  },
  "ram": {
    "total": 4134,
    "used": 2612,
    "usage": 63.18
  },
  "disk": {
    "read": 1048576.0,
    "write": 524288.0,
    "io": 4.2
  },
  "processes": [
    {
      "pid": 1,
      "name": "systemd",
      "state": "S",
      "user": "root"
    },
    {
      "pid": 1337,
      "name": "resource-monitor",
      "state": "R",
      "user": "dyzeex"
    },
    {
      "pid": 2048,
      "name": "kworker/u4:1",
      "state": "I",
      "user": "root"
    }
  ]
}
```

## Frontend/  
Frontend built uisng [Vite](https://vite.dev/). Electron framework, uPlot library and HTML/CSS/TypeScript given a WebUI app.

### `electron/main.ts`
Electron entry point. Creates an application window and runs the C++ backend as a child process.

**BACKEND_PATH** - path to compiled C++ binary.  
**startBackend()** - starts C++ process via **child_process.spawn()**.  
**createWindow()** - creates a BrowserWindow and loads the UI.  

**Operating modes:**
- *NODE_ENV=development* - loads the Vite dev server (`http://localhost:5173`) and opens DevTools;
- Production - loads the built `dist/index.html`.  
**Backend failure handling:** 
If the C++ process exits with a non-zero exit code - automatic restart after 3 seconds.
 
**Security:**
- `nodeIntegration: false` - renderer has no access to Node.js API;
- `contextIsolation: true` - isolation of contexts between main and renderer.

**Application events:**
- `before-quit` - kills the C++ process before closing;
- `window-all-closed` - ends application.

### `src/types.ts`
**Purpose:** describes structure of JSON packets that are sent by the C++ backend. TypeScript uses these types to validate data during compilation phase.
 
**Interfaces:**
 
```typescript
Process {
 pid: number // Process ID
 name: string // Process Name
 state: string // State (R/S/Z/... )
 user: string // Process owner
}
 
SystemData {
 cpu.usage: number // CPU usage in %
 ram.total: number // Total RAM in MB
 ram.used: number // Used RAM in MB
 ram.usage: number // RAM usage in %
 processes: Process[] // List of processes
 disk.read: number // Read speed in bytes/sec
 disk.write: number // Write speed in bytes/sec
 disk.io: number // I/O utilization in %
}
```

### `src/ws.ts`
Establishes and maintains a WebSocket connection with the C++ backend. If the connection is broken, it automatically reconnects.
 
#### `MonitorWebSocket` class  
*url* - Address of the WebSocket server (`ws://localhost:2006/ws`);  
*onData* - Callback function that is called for each incoming packet;  
*reconnectTimer* - Timer for auto-reconnect;  
*connect()* - Establishes a connection and attaches event handlers;  
*disconnect()* - Closes the connection and cancels the reconnect timer.
 
**Connection lifecycle:**
```
connect() → onopen → receiving data via onmessage → onclose → setTimeout(connect, 3000) → ...
```

Each incoming message is parsed using **JSON.parse()** and cast to the **SystemData**. type. If parsing fails, a console message is displayed, and the connection is not terminated.

### `src/charts.ts`
Making graphs for every backend component with [uPlot](https://github.com/leeoniya/uPlot).

#### `RingBuffer` class
 
*push(value)* - Adds a new value, overwriting the old one;  
*getValues()* - Returns all values in chronological order;  
*getTimeStamps()* - Returns an array of Unix timestamps for X axis (the last 60 seconds).  
 
How it works: **index** points to next position to write to and moves in a circular fashion. **getValues()** collects data from **index** to the end of buffer and then from beginning to **index**, which gives correct chronological order.
 
**Chart creation functions:**  

**createCPUChart(container)**
- One data row: CPU usage in % (red `#ff6b6b`)
- Y-axis: 0–100%
- Returns: `{ chart, buffer }`  
**createRAMChart(container)**
- One data row: RAM load in % (cyan `#4ecdc4`)
- Y-axis: 0–100%
- Returns: `{ chart, buffer }`  
**createDiskChart(container)**
- Two data rows: Read MB/s (purple `#a29b

### `src/main.ts`
#### Initialization
 
1. Creates three graphs: CPU, RAM, and Disk;
2. Sets a **ResizeObserver** on each container, so that when the window is resized, graphs automatically adjust their width;
3. Attaches handlers to table headers (sorting) and filter field;
4. Creates a **MonitorWebSocket** and connects to backend.

Function **handleData(data: SystemData)**
 
Called for each incoming WebSocket packet (every 500 ms):
 
| Block | Actions |
|---|---|
| CPU | Updates text value, pushes to buffer, redraws chart |
| RAM | Updates `used / total MB`, pushes percentage to buffer, redraws |
| Disk | Converts bytes/sec to MB/sec, updates Read/Write/IO text, redraws |
| Processes | Saves list, updates counter, and calls `renderTable()` |
| Time | Updates last update time in the header |
 
Function **renderTable()**
 
1. Copies `allProcesses` to avoid mutating the original;
2. Filters by `filterText` (name search, case-insensitive);
3. Sorts by `currentSort.field` and `currentSort.direction`;
4. Renders rows using `innerHTML`.

## Build

``` bash
git clone github.com/DyzeeX/resource-monitor.git
```
Before build, make sure that you have the following components downloaded:
[CMake](https://github.com/Kitware/CMake)
[OpenSSL](https://openssl-library.org/)
[npm](https://www.npmjs.com/)  

``` bash
cd Backend
mkdir build && cd build
cmake ..
cmake --build .

cd ../../Frontend
npm install

sudo chown root:root node_modules/electron/dist/chrome-sandbox
sudo chmod 4755 node_modules/electron/dist/chrome-sandbox

npm run build

npm run start
```

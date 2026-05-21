import './style.css'
import 'uplot/dist/uPlot.min.css'
import { MonitorWebSocket } from './ws'
import { createCPUChart, createRAMChart, createDiskChart, updateChart, updateDiskChart } from './charts'
import type { SystemData, Process } from './types'

const cpuContainer = document.getElementById('cpu-chart')!
const ramContainer = document.getElementById('ram-chart')!
const diskContainer = document.getElementById('disk-chart')!

const { chart: cpuChart, buffer: cpuBuffer } = createCPUChart(cpuContainer)
const { chart: ramChart, buffer: ramBuffer } = createRAMChart(ramContainer)
const { chart: diskChart, readBuffer, writeBuffer } = createDiskChart(diskContainer)

let allProcesses: Process[] = []
let filterText = ''
let currentSort = { field: 'pid', direction: 'asc' as 'asc' | 'desc' }

function handleData(data: SystemData) {
  
  // CPU
  const cpu = data.cpu.usage
  document.getElementById('cpu-value')!.textContent = `${cpu.toFixed(1)}%`
  updateChart(cpuChart, cpuBuffer, cpu)

  // RAM
  const ramUsed = data.ram.used
  const ramTotal = data.ram.total
  const ramPercent = data.ram.usage
  document.getElementById('ram-value')!.textContent = `${ramUsed} / ${ramTotal} MB`
  document.getElementById('ram-total')!.textContent = `${ramTotal} MB`
  updateChart(ramChart, ramBuffer, ramPercent)

  // Disk
  const diskRead = data.disk.read / (1024 * 1024)
  const diskWrite = data.disk.write / (1024 * 1024)
  const diskIO = data.disk.io

  document.getElementById('disk-read')!.textContent = `${diskRead.toFixed(2)} MB/s`
  document.getElementById('disk-write')!.textContent = `${diskWrite.toFixed(2)} MB/s`
  document.getElementById('disk-io')!.textContent = `${diskIO.toFixed(1)}%`
  updateDiskChart(diskChart, readBuffer, writeBuffer, diskRead, diskWrite)

  allProcesses = data.processes
  document.getElementById('process-count')!.textContent = String(data.processes.length)
  renderTable()
  
  document.getElementById('update-time')!.textContent =
    new Date().toLocaleTimeString('ru-RU')
}

function renderTable() {
  let list = [...allProcesses]

  if (filterText.trim()) {
    list = list.filter(p =>
      p.name.toLowerCase().includes(filterText.toLowerCase())
    )
  }

  list.sort((a, b) => {
    const aVal = a[currentSort.field as keyof Process]
    const bVal = b[currentSort.field as keyof Process]

    if (typeof aVal === 'number' && typeof bVal === 'number') {
      return currentSort.direction === 'asc' ? aVal - bVal : bVal - aVal
    }

    const aStr = String(aVal).toLowerCase()
    const bStr = String(bVal).toLowerCase()
    return currentSort.direction === 'asc'
      ? aStr.localeCompare(bStr)
      : bStr.localeCompare(aStr)
  })

  const tbody = document.getElementById('process-list')!
  tbody.innerHTML = list
    .map(p => `
      <tr>
        <td>${p.pid}</td>
        <td>${p.name}</td>
        <td>${p.user}</td>
        <td>${p.state}</td>
      </tr>
    `)
    .join('')
}

document.querySelectorAll('th[data-sort]').forEach(th => {
  th.addEventListener('click', () => {
    const field = th.getAttribute('data-sort')!

    if (currentSort.field === field) {
      currentSort.direction = currentSort.direction === 'asc' ? 'desc' : 'asc'
    } else {
      currentSort.field = field
      currentSort.direction = 'asc'
    }

    document.querySelectorAll('th[data-sort]').forEach(h => {
      h.classList.remove('asc', 'desc')
    })
    th.classList.add(currentSort.direction)

    renderTable()
  })
})

document.getElementById('process-filter')!.addEventListener('input', (e) => {
  filterText = (e.target as HTMLInputElement).value
  renderTable()
})

new MonitorWebSocket('ws://localhost:2006/ws', handleData)
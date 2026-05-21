import './style.css'
import 'uplot/dist/uPlot.min.css'
import { MonitorWebSocket } from './ws'
import { createCPUChart, createRAMChart, updateChart } from './charts'
import type { SystemData, Process } from './types'

const cpuContainer = document.getElementById('cpu-chart')!
const ramContainer = document.getElementById('ram-chart')!

const { chart: cpuChart, buffer: cpuBuffer } = createCPUChart(cpuContainer)
const { chart: ramChart, buffer: ramBuffer } = createRAMChart(ramContainer)

let allProcesses: Process[] = []
let filterText = ''
let currentSort = { field: 'pid', direction: 'asc' as 'asc' | 'desc' }

function handleData(data: SystemData) {
  
  const cpu = data.cpu.usage
  document.getElementById('cpu-value')!.textContent = `${cpu.toFixed(1)}%`
  updateChart(cpuChart, cpuBuffer, cpu)

  const ramUsed = data.ram.used
  const ramTotal = data.ram.total
  const ramPercent = data.ram.usage
  document.getElementById('ram-value')!.textContent = `${ramUsed} / ${ramTotal} MB`
  document.getElementById('ram-total')!.textContent = `${ramTotal} MB`
  updateChart(ramChart, ramBuffer, ramPercent)

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
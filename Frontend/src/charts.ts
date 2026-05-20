import uPlot from 'uplot'

export class RingBuffer {
  private buffer: number[]
  private maxSize: number
  private index: number = 0

  constructor(maxSize: number = 60) {
    this.maxSize = maxSize
    this.buffer = new Array(maxSize).fill(0)
  }

  push(value: number) {
    this.buffer[this.index] = value
    this.index = (this.index + 1) % this.maxSize
  }

  getValues(): number[] {
    return [
      ...this.buffer.slice(this.index),
      ...this.buffer.slice(0, this.index)
    ]
  }

  getTimeStamps(): number[] {
    return Array.from({ length: this.maxSize }, (_, i) => i)
  }
}

export function createCPUChart(container: HTMLElement) {
  const buffer = new RingBuffer(60)

  const opts: uPlot.Options = {
    title: '',
    width: container.offsetWidth || 400,
    height: 180,
    series: [
      {},
      {
        label: 'CPU %',
        stroke: '#ff6b6b',
        fill: '#ff6b6b22',
        width: 2,
        points: { show: false }
      }
    ],
    axes: [
      {
        stroke: '#555',
        grid: { stroke: '#333', width: 0.5 }
      },
      {
        stroke: '#555',
        grid: { stroke: '#333', width: 0.5 },
        scale: 'y'
      }
    ],
    scales: {
      x: { min: 0, max: 60 },
      y: { min: 0, max: 100 }
    }
  }

  const chart = new uPlot(opts, [buffer.getTimeStamps(), buffer.getValues()], container)

  return { chart, buffer }
}

export function createRAMChart(container: HTMLElement) {
  const buffer = new RingBuffer(60)

  const opts: uPlot.Options = {
    title: '',
    width: container.offsetWidth || 400,
    height: 180,
    series: [
      {},
      {
        label: 'RAM %',
        stroke: '#4ecdc4',
        fill: '#4ecdc422',
        width: 2,
        points: { show: false }
      }
    ],
    axes: [
      {
        stroke: '#555',
        grid: { stroke: '#333', width: 0.5 }
      },
      {
        stroke: '#555',
        grid: { stroke: '#333', width: 0.5 },
        scale: 'y'
      }
    ],
    scales: {
      x: { min: 0, max: 60 },
      y: { min: 0, max: 100 }
    }
  }

  const chart = new uPlot(opts, [buffer.getTimeStamps(), buffer.getValues()], container)

  return { chart, buffer }
}

export function updateChart(chart: uPlot, buffer: RingBuffer, value: number) {
  buffer.push(value)
  chart.setData([buffer.getTimeStamps(), buffer.getValues()])
}
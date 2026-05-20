import type { SystemData } from './types'

type DataCallback = (data: SystemData) => void

export class MonitorWebSocket {
  private ws: WebSocket | null = null
  private url: string
  private onData: DataCallback
  private reconnectTimer: number | null = null

  constructor(url: string, onData: DataCallback) {
    this.url = url
    this.onData = onData
    this.connect()
  }

  private connect() {
    console.log('Connecting to ', this.url)

    this.ws = new WebSocket(this.url)

    this.ws.onopen = () => {
      console.log('WebSocket is on!')
      if (this.reconnectTimer) {
        clearTimeout(this.reconnectTimer)
        this.reconnectTimer = null
      }
    }

    this.ws.onmessage = (event: MessageEvent) => {
      try {
        const data: SystemData = JSON.parse(event.data)
        this.onData(data)
      } catch (err) {
        console.error('Parse error JSON:', err)
      }
    }

    this.ws.onerror = (err) => {
      console.error('WebSocket error:', err)
    }

    this.ws.onclose = () => {
      console.log('WebSocket is off, reconnect in 3 secs ...')
      this.reconnectTimer = window.setTimeout(() => this.connect(), 3000)
    }
  }

  public disconnect() {
    if (this.reconnectTimer) clearTimeout(this.reconnectTimer)
    if (this.ws) this.ws.close()
  }
}
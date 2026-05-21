export interface Process {
  pid: number
  name: string
  state: string
  user: string
}

export interface SystemData {
  cpu: {
    usage: number
  }
  ram: {
    total: number
    used: number
    usage: number
  }
  processes: Process[]
  disk: {
    read: number
    write: number
    io: number 
  }
}
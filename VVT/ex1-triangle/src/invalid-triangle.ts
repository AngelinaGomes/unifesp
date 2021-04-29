export class InvalidTriangle extends Error {
  constructor (message: string) {
    super(message)
    this.name = 'InvalidTriangle'
  }
}

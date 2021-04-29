export class Identifier {
  private readonly identifier: string
  constructor (identifier: string) {
    this.identifier = identifier
  }

  isValid (): boolean {
    const regExp = /^[a-zA-Z][a-zA-Z0-9]*$/
    if (regExp.test(this.identifier) &&
        this.identifier.length >= 1 &&
        this.identifier.length <= 6) {
      return true
    } else {
      return false
    }
  }
}

import { Identifier } from './identifier'

describe('classify identifier', () => {
  test('should validate identifier', () => {
    const id1 = new Identifier('id123')
    const ret = id1.isValid()
    expect(ret).toBe(true)
  })

  test('should return false if identifier do not starts with letter', () => {
    const id1 = new Identifier('123id')
    const ret = id1.isValid()
    expect(ret).toBe(false)
  })

  test('should return false if identifier contains invalid characters', () => {
    const id1 = new Identifier('id#')
    const ret = id1.isValid()
    expect(ret).toBe(false)
  })

  test('should return false if identifier size is smaller than 1', () => {
    const id1 = new Identifier('')
    const ret = id1.isValid()
    expect(ret).toBe(false)
  })

  test('should return false if identifier size is bigger than 6', () => {
    const id1 = new Identifier('identifier')
    const ret = id1.isValid()
    expect(ret).toBe(false)
  })
})

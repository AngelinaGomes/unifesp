import { InvalidTriangle } from './invalid-triangle'
import { Triangle, TriangleType } from './triangle'

describe('triangle class', () => {
  test('shoud throw error for invalid triangle: negative side', () => {
    expect(() => {
      const t1 = new Triangle(-2, -3, -4)
      console.log(t1)
    }).toThrow(new InvalidTriangle('Invalid triangle.'))
  })

  test('shoud throw error for invalid triangle: sum of two sides smaller than the other', () => {
    expect(() => {
      const t1 = new Triangle(2, 3, 5)
      console.log(t1)
    }).toThrow(new InvalidTriangle('Invalid triangle.'))
  })

  test('shoud correctly classify an equilateral triangle', () => {
    const t1 = new Triangle(3, 3, 3)
    const ret = t1.classify()
    expect(ret).toEqual(TriangleType.EQUILATERAL)
  })

  test('shoud correctly classify an scalene triangle', () => {
    const t1 = new Triangle(2, 3, 4)
    const ret = t1.classify()
    expect(ret).toEqual(TriangleType.SCALENE)
  })

  test('shoud correctly classify an isosceles triangle', () => {
    const t1 = new Triangle(2, 3, 2)
    const ret = t1.classify()
    expect(ret).toEqual(TriangleType.ISOSCELES)
  })
})

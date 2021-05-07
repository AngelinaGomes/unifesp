import { calculatesIR } from './tax'

describe('income tax tests', () => {
  test('shold return 0 - to values below 1000', () => {
    const ret = calculatesIR(999)
    expect(ret).toEqual(0)
  })
  test('should return (salary-1000)*15% - to values between 1000 and 2000', () => {
    const ret = calculatesIR(1001)
    expect(ret).toEqual(0.15)
  })
  test('should return 150+(salary-2000)*20% - to values above 2000', () => {
    const ret = calculatesIR(2050)
    expect(ret).toEqual(160)
  })
  test('should return 150+(salary-2000)*20% + (salary-3000)*5% - to values above 3000', () => {
    const ret = calculatesIR(3050)
    expect(ret).toEqual(362.5)
  })
})

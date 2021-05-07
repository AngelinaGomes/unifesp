// The income tax (IR - Imposto de Renda) is calculated as follows:
//   if salary > 2000, IR = (150) + (salary - 2000) * 20%
//   if 1000 < salary <= 2000, IR = (salary - 1000) * 15%
//   if salary <= 1000, IR = 0

export function calculatesIR (salary: number): number {
  let ret: number = 0.0
  if (salary > 1000.0 && salary <= 2000.0) {
    ret += (salary - 1000.0) * (0.15)
  }
  if (salary > 2000.0) {
    ret += 150 + (salary - 2000.0) * (0.20)
  }
  if (salary > 3000.0) {
    ret += (salary - 3000.0) * (0.05)
  }
  return ret
}

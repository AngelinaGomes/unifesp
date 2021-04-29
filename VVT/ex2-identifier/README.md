# Equivalence Class Partitioning

---

"Defina classes de equivalência e um conjunto de casos de teste adequado para o critério particionamento por classes de equivalência para o programa com a seguinte descrição:

O programa deve determinar se um identificador é válido ou não em Silly Pascal (uma variante do Pascal). Um identificador válido deve <<começar com uma letra e conter apenas letras ou dígitos>>. Além disso, <<deve ter no mínimo um caractere e no máximo seis caracteres de comprimento>>.

Implemente a função isValid(string): boolean que recebe uma string e retorna verdadeiro se a string pode ser um identificador de Silly Pascal e falso caso contrário. Crie os testes para essa função."

---

| Input Condition  | Valid Classes | Invalid Classes |
| ------------- | ------------- | ------------- |
| starts with letter | yes **(1)** | no **(2)** |
| only letter or numbers  | yes **(3)** | no **(4)** |
| identifier size  | 1 ≤ &#124;id&#124; ≤ 6 **(5)** | &#124;id&#124; = 0 **(6)** |
|  |  | &#124;id&#124; > 6 **(7)** |

| Test Cases | Covered Classes |
| ------------- | ------------- |
| ("id123", true) | 1, 3, 5 |
| ("123id", false) | 2 |
| ("id#", false) | 4 |
| ("", false) | 6 |
| ("identifier", false) | 7 |

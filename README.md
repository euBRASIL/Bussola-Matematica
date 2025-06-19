# Mapeador Cartesiano de Números em C

## Visão Geral

Este projeto fornece um programa em C (`mapper.c`) que mapeia números inteiros grandes (unsigned long) para coordenadas Cartesianas 2D (x, y). O mapeamento é baseado em uma interpretação geométrica onde cada número corresponde a um vetor polar, que é então convertido para coordenadas Cartesianas. Esta abordagem utiliza uma espiral angular predefinida e valores trigonométricos pré-calculados para evitar bibliotecas matemáticas externas.

A lógica principal está encapsulada na função `map_to_cartesian` dentro de `mapper.c`.

## Processo Matemático

O mapeamento de um número de entrada para coordenadas Cartesianas segue estes passos:

1.  **Número de Entrada (`N`)**: Um inteiro `unsigned long`.

2.  **Componente Angular (Determinação de Theta)**:
    *   O sistema usa uma espiral de 40 vetores angulares discretos, cada um separado por 9 graus (40 * 9° = 360°).
    *   O vetor específico para o número de entrada `N` é determinado pelo seu resto quando dividido por 40:
        `indice_theta = N % 40`
    *   Este `indice_theta` é então usado para consultar valores de seno e cosseno pré-calculados. O ângulo real em graus seria `theta = indice_theta * 9`.

3.  **Componente Radial (Determinação do Raio)**:
    *   A magnitude (ou raio `r`) do vetor é determinada pelo quociente inteiro do número de entrada `N` dividido por 40:
        `raio = N / 40`

4.  **Cálculo das Coordenadas**:
    *   Com o `indice_theta` e `raio`, as coordenadas Cartesianas (x, y) são calculadas usando:
        *   `coordenada_x = raio * tabela_cos[indice_theta]`
        *   `coordenada_y = raio * tabela_sin[indice_theta]`
    *   `tabela_cos` e `tabela_sin` são arrays pré-calculados que armazenam os valores de cosseno e seno para cada um dos 40 ângulos (0°, 9°, 18°, ..., 351°).

## Detalhes da Implementação

*   **Linguagem**: C (padrão C99 assumido para os tipos `unsigned long` e `float`).
*   **Tipo do Número de Entrada**: `unsigned long`.
*   **Valores Trigonométricos**: Tipo `float`, armazenados em `tabela_sin` e `tabela_cos`.
*   **Coordenadas de Saída**: Tipo `float`.
*   **Dependências**: Biblioteca padrão C (`stdio.h` para impressão). Nenhuma biblioteca matemática externa (como `math.h` para funções `sin`/`cos` em tempo de execução) é usada para a conversão principal; os valores trigonométricos vêm de tabelas. A flag `-lm` durante a compilação é uma boa prática caso funções de `math.h` sejam adicionadas posteriormente, mas não é estritamente necessária para a atual consulta de `sin`/`cos` baseada em tabelas.

## Compilação

Para compilar o programa `mapper.c`, use um compilador C como o GCC:

```bash
gcc mapper.c -o mapper -lm
```
(O `-lm` vincula a biblioteca matemática, o que não é estritamente necessário para a versão atual que usa apenas tabelas, mas é inofensivo.)

## Execução

Após a compilação bem-sucedida, execute o programa:

```bash
./mapper
```

O programa usará um número de exemplo codificado no programa (`123456789`) e imprimirá os detalhes do mapeamento no console.

## Exemplo de Saída

A execução do `mapper` produzirá uma saída similar à seguinte (a precisão exata do ponto flutuante pode variar ligeiramente dependendo do sistema/compilador):

```
Input Number: 123456789
Theta Index: 29 (Angle: 261 degrees)
Radius: 3086419
Cos_val from table at index 29: -0.15643448
Sin_val from table at index 29: -0.98768836
Calculated X Coordinate: -482822.34375000
Calculated Y Coordinate: -3048420.25000000
```

Esta saída mostra o número de entrada, seu correspondente índice theta e ângulo, o raio calculado, os valores de seno/cosseno recuperados das tabelas para esse índice, e as coordenadas Cartesianas X e Y finais computadas.

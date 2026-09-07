# PickInteract Include

Uma biblioteca moderna, otimizada e orientada a eventos para **SA-MP /
open.mp** que permite a criação de pickups interativos com **TextDraws
dinâmicas de interface**, suporte a cores Hexadecimal e gerenciamento de
memória em tempo constante **O(1)** utilizando YSI Iterators e Streamer
Areas.

------------------------------------------------------------------------

## 📋 Sumário

-   [Visão Geral](#-visão-geral)
-   [Comparativo de Desempenho
    (Refatoração)](#-comparativo-de-desempenho-refatoração)
-   [Recursos](#-recursos)
-   [Dependências](#-dependências)
-   [Instalação](#-instalação)
-   [Configurações](#-configurações)
-   [Documentação da API](#-documentação-da-api)
-   [Exemplo Prático](#-exemplo-prático)
-   [Licença](#-licença)

------------------------------------------------------------------------

## 🔍 Visão Geral

O **PickInteract** resolve o problema de renderização de comandos visuais e
indicadores de interatividade no mundo 3D do jogo.

Ele cria automaticamente áreas esféricas acopladas aos pickups e exibe
interfaces personalizadas quando o jogador está próximo, destruindo-as
assim que ele se afasta ou desconecta.

A arquitetura é baseada em eventos do Streamer, evitando verificações
constantes através de `OnPlayerUpdate` ou timers.

------------------------------------------------------------------------

## ⚡ Comparativo de Desempenho (Refatoração)

  -------------------------------------------------------------------------
  Recurso / Arquitetura   Versão Antiga (v0.0.1)  Nova Versão (Refatorada)
  ----------------------- ----------------------- -------------------------
  **Gatilho de Detecção** `OnPlayerUpdate` (loop  `OnPlayerEnterDynArea` /
                          contínuo por jogador)   `OnPlayerLeaveDynArea`
                                                  (orientado a eventos)

  **Complexidade de       `O(N × M)` com buscas   `O(1)` indexado via
  Busca**                 radiais manuais         `E_STREAMER_EXTRA_ID`

  **Alocação de Memória** Array estático          `y_iterate` com reuso
                          incremental             automático de IDs livres

  **Customização Visual** Cor decimal fixa e      Cores Hexadecimal
                          idêntica para todos     (`#RRGGBB`) individuais

  **Destruição Dinâmica** Não suportado em tempo  Suportado via
                          de execução             `DestroyPickupInteract`

  **Mundos / Interiores** Limitado ao mundo       Suporte nativo a
                          global                  `worldid` e `interiorid`
  -------------------------------------------------------------------------

------------------------------------------------------------------------

## 🚀 Recursos

-   **Zero Polling Overhead:** Nenhum `timer` ou `OnPlayerUpdate`
    rodando em segundo plano.
-   **Gerenciamento Inteligente de TextDraws:** Criação e destruição sob
    demanda por jogador, prevenindo estouro do limite global de
    TextDraws.
-   **Cores Hexadecimal:** Suporte nativo à conversão de strings Hex
    (`#RRGGBB`) via `sscanf`.
-   **Modularidade:** Totalmente integrado com ganchos `y_hooks`,
    dispensando modificações diretas nas callbacks nativas do servidor.
-   **Áreas Dinâmicas:** Cada pickup possui uma área dinâmica associada
    para controlar a entrada e saída do jogador.
-   **Suporte a Virtual Worlds e Interiors:** Pickups e áreas podem ser
    vinculados a mundos virtuais e interiores específicos.

------------------------------------------------------------------------

## 📦 Dependências

Certifique-se de carregar os componentes e includes necessários:

1.  **samp-streamer-plugin**\
    https://github.com/samp-incognito/samp-streamer-plugin

2.  **YSI-Includes** (`y_iterate`, `y_hooks`)\
    https://github.com/pawn-lang/YSI-Includes

3.  **sscanf2**\
    https://github.com/Y-Less/sscanf

------------------------------------------------------------------------

## 📥 Instalação

### 1. Instale as dependências

Certifique-se de que o Streamer Plugin, YSI-Includes e sscanf2 estejam
corretamente instalados.

### 2. Adicione o arquivo

Mova o arquivo `PickInteract.inc` para:

``` text
pawno/include/
```

### 3. Inclua as dependências

No seu gamemode ou filtro, adicione:

``` pawn
#include <a_samp>
#include <streamer>
#include <sscanf2>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

#include <PickInteract>
```

> **Importante:** As dependências devem estar disponíveis antes da
> compilação do `PickInteract.inc`.

------------------------------------------------------------------------

## ⚙️ Configurações

Você pode definir limites customizados antes de incluir o PickInteract.

``` pawn
#define MAX_INTERACT_PICKUPS    (1000) // Padrão: 500
#define INTERACT_DISTANCE       (3.0)  // Padrão: 2.0

#include <PickInteract>
```

### `MAX_INTERACT_PICKUPS`

Define a quantidade máxima de pickups interativos que podem existir
simultaneamente.

Valor padrão:

``` pawn
#define MAX_INTERACT_PICKUPS (500)
```

### `INTERACT_DISTANCE`

Define a distância padrão utilizada para a área de interação.

Valor padrão:

``` pawn
#define INTERACT_DISTANCE (2.0)
```

Esse valor pode ser sobrescrito individualmente ao criar um pickup.

------------------------------------------------------------------------

# 📚 Documentação da API

## `AddPickupInteract`

Cria um pickup interativo e gera sua respectiva área de gatilho no
Streamer.

### Sintaxe

``` pawn
stock AddPickupInteract(
    modelid,
    Float:x,
    Float:y,
    Float:z,
    const key[],
    const color[] = "#DF5454",
    Float:distance = INTERACT_DISTANCE,
    worldid = -1,
    interiorid = -1
);
```

### Parâmetros

  -----------------------------------------------------------------------
  Parâmetro                           Descrição
  ----------------------------------- -----------------------------------
  `modelid`                           Modelo do pickup a ser criado.

  `x`                                 Coordenada X.

  `y`                                 Coordenada Y.

  `z`                                 Coordenada Z.

  `key[]`                             Caractere ou texto da tecla exibida
                                      na interface, como `"F"` ou `"E"`.

  `color[]`                           Cor hexadecimal do indicador
                                      visual.

  `distance`                          Raio da área de detecção.

  `worldid`                           Virtual World do pickup e da área.

  `interiorid`                        Interior ID do pickup e da área.
  -----------------------------------------------------------------------

### Retorno

Retorna o ID do pickup gerado ou:

``` pawn
INVALID_STREAMER_ID
```

em caso de falha.

### Exemplo

``` pawn
new pickupid;

pickupid = AddPickupInteract(
    1274,
    1520.0,
    -1670.0,
    13.5,
    "E",
    "#4CAF50",
    3.0
);
```

------------------------------------------------------------------------

## `DestroyPickupInteract`

Remove um pickup interativo e limpa sua área de detecção e seus dados
associados.

### Sintaxe

``` pawn
stock DestroyPickupInteract(index);
```

### Parâmetros

  -----------------------------------------------------------------------
  Parâmetro                           Descrição
  ----------------------------------- -----------------------------------
  `index`                             Índice retornado na criação ou
                                      posição utilizada pelo iterator.

  -----------------------------------------------------------------------

### Retorno

-   `1` --- removido com sucesso.
-   `0` --- índice inexistente ou pickup não encontrado.

### Exemplo

``` pawn
DestroyPickupInteract(pickupid);
```

------------------------------------------------------------------------

## `IsPlayerInAnyInteractArea`

Verifica se o jogador está atualmente dentro de alguma área de interação
ativa do include.

### Sintaxe

``` pawn
static stock bool:IsPlayerInAnyInteractArea(playerid);
```

### Parâmetros

  Parâmetro    Descrição
  ------------ ------------------------------------
  `playerid`   ID do jogador que será verificado.

### Retorno

-   `true` --- jogador está em uma área de interação.
-   `false` --- jogador não está em nenhuma área.

### Exemplo

``` pawn
if (IsPlayerInAnyInteractArea(playerid))
{
    // Jogador está em uma área interativa.
}
```

------------------------------------------------------------------------

# 💡 Exemplo Prático

Um exemplo simples criando um pickup interativo:

``` pawn
public OnGameModeInit()
{
    AddPickupInteract(
        1274,
        1520.0,
        -1670.0,
        13.5,
        "E",
        "#DF5454",
        2.5
    );

    return 1;
}
```

A partir desse momento, o PickInteract gerencia a área dinâmica associada ao
pickup.

Quando o jogador entrar na área, o sistema poderá criar a interface
visual correspondente.

Quando sair da área, a interface poderá ser destruída automaticamente.

------------------------------------------------------------------------

## 🧠 Arquitetura

O fluxo básico do PickInteract é:

``` text
Pickup Interativo
       │
       ▼
Streamer Dynamic Area
       │
       ├── Jogador entra
       │       │
       │       ▼
       │   Criação da UI
       │
       └── Jogador sai
               │
               ▼
           Destruição da UI
```

O relacionamento entre a área dinâmica e o pickup é armazenado
utilizando os recursos do Streamer, permitindo recuperar o índice
associado sem realizar uma busca radial por todos os pickups existentes.

Isso evita estruturas como:

``` text
para cada jogador
    para cada pickup
        calcular distância
```

e permite que a detecção seja conduzida pelos eventos de entrada e saída
das áreas dinâmicas.

------------------------------------------------------------------------

# 🧩 Integração com Sistemas

O PickInteract foi projetado para ser utilizado como uma camada de interação
para outros sistemas do servidor.

Por exemplo:

``` text
PickInteract
   │
   ├── Banco / ATM
   ├── Lojas
   ├── NPCs
   ├── Portas
   ├── Empregos
   ├── Caixas eletrônicos
   ├── Objetos interativos
   └── Sistemas personalizados
```

A biblioteca pode ser utilizada para indicar visualmente ao jogador que
existe uma interação disponível naquele local.

------------------------------------------------------------------------

# 📌 Boas Práticas

### Use cores diferentes para contextos diferentes

Exemplo:

``` pawn
"#4CAF50" // Interação positiva
"#2196F3" // Informação
"#FF9800" // Atenção
"#F44336" // Perigo
```

### Utilize distâncias menores para interações precisas

``` pawn
AddPickupInteract(1274, x, y, z, "E", "#4CAF50", 2.0);
```

Para interações que precisam ser percebidas de uma distância maior:

``` pawn
AddPickupInteract(1274,  x,  y, z, "E", "#4CAF50", 4.0);
```

------------------------------------------------------------------------

## 👤 Créditos

**PickInteract Include**
**Colaboradores
Kingsman Rlk
MMV-DEV
**

Biblioteca desenvolvida para projetos **SA-MP / open.mp**, com foco em
performance, modularidade e interações orientadas a eventos.

Se este projeto for distribuído como fork, preserve os créditos dos
autores originais e indique claramente as modificações realizadas.

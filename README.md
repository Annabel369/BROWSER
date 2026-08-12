# DOS Web Browser 🌐🖥️
<img width="1024" height="1028" alt="Gemini_Generated_Image_ld4deld4deld4del" src="https://github.com/user-attachments/assets/484382fa-8ebe-4286-a9c0-b9384af23b90" />


Um projeto retrô e nostálgico: um navegador web (web browser) básico feito inteiramente em **C++** para rodar em ambientes **MS-DOS**, utilizando o lendário compilador **Borland C++ 3.1** e a biblioteca gráfica **BGI** (Borland Graphics Interface).

## ✨ Funcionalidades

- **Parser HTML Embutido:** Lê e interpreta arquivos locais (`.HTM` ou `.HTML`).
- **Renderização Gráfica:** Transforma as tags HTML em elementos visuais na tela usando a biblioteca `<graphics.h>`.
- **Suporte a Mouse:** Interrupções de hardware (INT 33h) para capturar o clique esquerdo e identificar interações na tela.
- **Navegação Real:** Permite a transição entre páginas através de hiperlinks clicáveis.

### Tags HTML Suportadas
O parser atual suporta a renderização básica das seguintes tags:
- `<h1>` / `<H1>`: Título principal (fonte sans-serif amarela).
- `<p>` / `<P>`: Parágrafo de texto normal (fonte branca).
- `<br>` / `<BR>`: Quebra de linha.
- `<a>` / `<A>`: Hiperlink interativo (texto azul e clicável). Suporta apontar para outras páginas locais ou `exit` para sair do programa.

## 📁 Estrutura de Arquivos

- `BROWSER.CPP`: Código-fonte principal com a engine de Layout, DOM estrutural, Interrupções de Mouse e o Parser HTML.
- `PAGE1.HTM`, `PAGE2.HTM`, `PAGE3.HTM`: Páginas de teste pré-construídas para navegar e testar a troca de contexto.
- `COMPILE.BAT`: Script batch para compilar rapidamente por linha de comando no DOS.

## 🛠️ Como Compilar

Para compilar, você vai precisar do **Borland C++ 3.1** configurado no seu emulador (como o DOSBox) ou em uma máquina retrô real.

### Via Linha de Comando (BCC)
Você pode utilizar o utilitário de linha de comando passando a flag de *Large memory model* (`-ml`) e incluindo a biblioteca de gráficos:

```bat
C:\BCPP31\BIN\BCC.EXE -ml -IC:\BCPP31\INCLUDE -LC:\BCPP31\LIB BROWSER.CPP C:\BCPP31\LIB\GRAPHICS.LIB
```

### Via IDE do Borland C++ (`BC.EXE`)
1. Abra o `BC.EXE`.
2. Vá em **Options** -> **Directories...** e configure:
   - **Include Directories:** `C:\BCPP31\INCLUDE`
   - **Library Directories:** `C:\BCPP31\LIB`
3. Vá em **Options** -> **Linker** -> **Libraries...** e marque a caixa **Graphics library**.
4. Aperte `F9` para compilar o projeto (Make).

## 🚀 Como Executar

Execute o `BROWSER.EXE` no seu ambiente DOS.
O programa tentará inicializar automaticamente a resolução VGA chamando o driver da BGI e em seguida abrirá a `PAGE1.HTM` que deve estar no mesmo diretório do executável.

**Nota:** Garanta que o driver do mouse (tipo o `mouse.com` do DOSBox ou FreeDOS) esteja em execução no sistema para interagir com a interface.

---
*Este projeto foi criado como uma prova de conceito de como os primeiros sistemas interpretavam textos estruturados para criar interfaces dinâmicas!*

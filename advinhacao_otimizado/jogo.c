#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"
#include "historico.h"
#include "../interfaces/text_utils.h"

static const int CONSOLE_WIDTH = 120;

static void clear_line(int y)
{
    gotoxy(1, y);
    for (int i = 0; i < CONSOLE_WIDTH; ++i)
        putchar(' ');
}

static void wait_for_enter(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
    ch = getchar();
    while (ch != '\n' && ch != EOF)
        ch = getchar();
}

Session executar_partida()
{
    Session s = {0};
    s.target = (rand() % 100) + 1; // RF01 [cite: 19]

    clear_screen();
    print_title_bar("LuckyGuess - Jogo", BOLD_BLUE, BOLD_WHITE);

    // Simulação de timestamp simples [cite: 30]
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(s.timestamp, sizeof(s.timestamp), "%Y-%m-%d %H:%M:%S", &tm);

    gotoxy(4, 5);
    print_colored("Novo jogo iniciado! Tente adivinhar o numero.", WHITE);
    gotoxy(4, 6);
    print_colored("Digite um palpite entre 1 e 100.", DARK_GRAY);

    while (s.attempts_count < MAX_GUESSES)
    {
        int palpite;
        clear_line(8);
        gotoxy(4, 8);
        printf("Palpite (%d/%d): ", s.attempts_count + 1, MAX_GUESSES);
        scanf("%d", &palpite);

        if (palpite < 1 || palpite > 100)
        {
            clear_line(10);
            gotoxy(4, 10);
            print_colored("Palpite invalido. Tente novamente.", YELLOW);
            continue; // RF02 [cite: 21]
        }

        s.guesses[s.attempts_count++] = palpite;

        if (palpite < s.target)
        {
            clear_line(10);
            gotoxy(4, 10);
            print_colored("Muito baixo!", BRIGHT_CYAN);
            s.low_count++;
        }
        else if (palpite > s.target)
        {
            clear_line(10);
            gotoxy(4, 10);
            print_colored("Muito alto!", BRIGHT_MAGENTA);
            s.high_count++;
        }
        else
        {
            clear_line(10);
            gotoxy(4, 10);
            printf("Voce acertou, depois de %d tentativas.", s.attempts_count);
            clear_line(12);
            gotoxy(4, 12);
            print_colored("Pressione ENTER para ver as estatisticas...", DARK_GRAY);
            wait_for_enter();
            break;
        }
    }

    salvar_sessao(s); // RF03 [cite: 27]
    return s;
}
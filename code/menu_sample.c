/** A simple console menu sample.
@author Wanderley Caloni <wanderley.caloni@gmail.com>
@date 2018-08
*/

#include <stdio.h>

int main(int argc, char* argv[])
{
    int ret = 0;

    printf("Exemplo de Menu\n"
           "===============\n");

    char opt;

    do
    {
        printf("\n"
                "=====\n"
                "[1] Opcao 1\n"
                "[2] Opcao 2.\n"
                "[3] Opcao 3.\n"
                "[4] Opcao 4.\n"
                "[0] Terminar.\n\n");

        opt = (char) getchar();
        getchar(); // enter

        switch (opt)
        {
            case '0':
                printf("Entao adeus =)\n");
                break;

            case '1':
                printf("Voce digitou Opcao 1\n");
                break;

            case '2':
                printf("Voce digitou Opcao 2\n");
                break;

            case '3':
                printf("Voce digitou Opcao 3\n");
                break;

            case '4':
                printf("Voce digitou Opcao 4\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }

    } while (opt != '0');

    return ret;
}


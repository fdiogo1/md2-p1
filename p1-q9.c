#include <stdio.h>

// Calcular euclidesMDC - Euclides
int euclidesMDC(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Verifica se dois números sao primos entre si (coprimos)
int verificarCoprimos(int a, int b) {
    return euclidesMDC(a, b) == 1;
}

// Verifica se um número eh primo (usando euclidesMDC com todos menores que ele)
int verificarPrimo(int n) {
    if (n < 2) return 0;
    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Calcula o inverso modular de G mod n usando o Algoritmo Estendido de Euclides
int inverso_modular(int G, int n) {
    int t = 0, novo_t = 1;
    int r = n, novo_r = G;

    while (novo_r != 0) {
        int quociente = r / novo_r;
        int temp = novo_t;
        novo_t = t - quociente * novo_t;
        t = temp;

        temp = novo_r;
        novo_r = r - quociente * novo_r;
        r = temp;
    }

    if (r > 1) return -1; // Inverso nao existe
    if (t < 0) t += n;

    return t;
}

// Calcular potência modular eficiente
long long potModular(long long base, long long exp, long long mod) {
    long long resultado = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            resultado = (resultado * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return resultado;
}

// Função φ(n)
int functionPhi(int n) {
    int resultado = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            resultado -= resultado / i;
        }
    }
    if (n > 1)
        resultado -= resultado / n;
    return resultado;
}

int main() {
    int H, G, n, x, n1;
    printf("Insira os valores na seguinte ordem: H G n x n1:\n");
    scanf("%d %d %d %d %d", &H, &G, &n, &x, &n1);

    printf("\nPasso 1: Verificar se G e n sao coprimos:\n");
    if (!verificarCoprimos(G, n)) {
        printf("G (%d) e n (%d) nao sao coprimos. Divisao modular naoo eh possivel.\n", G, n);
        return 1;
    }
    printf("Conclusao: G (%d) e n (%d) sao coprimos.\n", G, n);

    printf("\nPasso 2: Calcular o inverso de G em Z%d\n", n);
    int G_inv = inverso_modular(G, n);
    if (G_inv == -1) {
        printf("O inverso modular de G (%d) nao existe em Z%d.\n", G, n);
        return 1;
    }
    printf("Inverso de %d em Z%d eh: %d\n", G, n, G_inv);

    printf("\nPasso 3: Calcular a = H * G^(-1) mod n\n");
    int a = (H * G_inv) % n;
    printf("a = %d\n", a);

    printf("\nPasso 4: Verificar se a e n1 sao coprimos\n");
    if (!verificarCoprimos(a, n1)) {
        printf("a (%d) e n1 (%d) nao sao coprimos. Operacao nao pode prosseguir.\n", a, n1);
        return 1;
    }
    printf("a (%d) e n1 (%d) sao coprimos.\n", a, n1);

    printf("\nPasso 5: Verificar se n1 eh primo\n");
    int x1;
    if (verificarPrimo(n1)) {
        printf("n1 (%d) eh primo. Logo, pode-se aplicar o Pequeno Teorema de Fermat.\n", n1);
        x1 = n1 - 1;
    } else {
        printf("n1 (%d) nao eh primo. Optando pelo Teorema de Euler.\n", n1);
        x1 = functionPhi(n1);
        printf("φ(%d) = %d\n", n1, x1);
    }

    printf("\nPasso 6: Decompor x na forma x = x1 * q + r\n");
    int q = x / x1;
    int r = x % x1;
    printf("x = %d = %d * %d + %d\n", x, x1, q, r);

    printf("\nPasso 7: Calcular potencias modulares intermediarias\n");
    long long x2 = potModular(a, x1, n1);
    long long x2q = potModular(x2, q, n1);
    long long ar = potModular(a, r, n1);

    printf("a^x1 mod n1 = %lld\n", x2);
    printf("(a^x1)^q mod n1 = %lld\n", x2q);
    printf("a^r mod n1 = %lld\n", ar);

    long long resultado_final = (x2q * ar) % n1;
    printf("\nResposta final: ((%lld)^%d * %lld) mod %d = %lld ---\n", x2, q, ar, n1, resultado_final);

    return 0;
}
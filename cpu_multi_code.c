/**
 * Aluno: Bruno Ricardo Siqueira
 * nUSP : 4953672
 * Grupo: 13
 */

#define INT_MAX 2147483647
#define INT_MIN -2147483648
#include"mascara.h"

/**
 * Especificação:
 * ULA deve seguir a especificação dada em sala de aula.
 * As funcoes necessarias a ULA devem estar implementadas em ula_code.c, cujo "#include" encontra-se logo abaixo
 * int ula (int a, int b, char ula_op, int *result_ula, char *zero, char *overflow)
 * args de entrada: int a, int b, char ula_op
 * args de saida: int *result_ula, char *zero, char *overflow
 **/
 /*
int ula (int a, int b, char ula_op, int *result_ula, char *zero, char *overflow)
{
    switch (ula_op) {
        case 0x00:
            *result_ula = a & b; // and
            *overflow = 0x00;
            break;
        case 0x01:
            *result_ula = a | b; // or
            *overflow = 0x00;
            break;
        case 0x02:
            *result_ula = a + b; // add
            if ((a^b) < 0) {
                *overflow = 0x00;
            } else if (a > 0) {
                if (b > INT_MAX - a) *overflow = 0x01;
                else overflow = 0x00;
            } else {
                if (b < INT_MIN - a) *overflow = 0x01;
                else *overflow = 0x00;
            }
            break;
        case 0x06:
            *result_ula = a - b; // sub
            if ((a^(-b)) < 0) {
                *overflow = 0x00;
            } else if (a > 0) {
                if ((-b) > INT_MAX - a) *overflow = 0x01;
                else *overflow = 0x00;
            } else {
                if ((-b) < INT_MIN - a) *overflow = 0x01;
                else *overflow = 0x00;
            }
            break;
        case 0x07:
            *result_ula = a < b ? 1 : 0; // slt
            *overflow = 0x00;
            break;
        default:
            *result_ula = 0x00; // add
            *overflow = 0x00;
            break;
    }

    if (*result_ula == 0x00) *zero = 0x01;
    else *zero = 0x00;

    return;
}*/

/**
 * Especificação:
 * UC principal
 * void UnidadeControle(int IR, short int *sc);
 * args de entrada: * int IR
 * args de saida: * short int *sc
 **/
void UnidadeControle(int IR, int *sc)
{
    int rom_controle[10][2], rom_despacho_1[5][2], rom_despacho_2[2][2];

    /**
     * matriz emulando a memoria ROM com os sinais de controle e CtlEnd
     **/
    rom_controle[0][0] = 0b1001010000001000 ;
    rom_controle[0][1] = 0b11;
    rom_controle[1][0] = 0b0000000000011000;
    rom_controle[1][1] = 0b01;
    rom_controle[2][0] = 0b0000000000010100;
    rom_controle[2][1] = 0b10;
    rom_controle[3][0] = 0b0011000000000000;
    rom_controle[3][1] = 0b11;
    rom_controle[4][0] = 0b0000001000000010 ;
    rom_controle[4][1] = 0b00;
    rom_controle[5][0] = 0b0010100000000000;
    rom_controle[5][1] = 0b00;
    rom_controle[6][0] = 0b0000000001000100;
    rom_controle[6][1] = 0b11;
    rom_controle[7][0] = 0b0000000000000011;
    rom_controle[7][1] = 0b00;
    rom_controle[8][0] = 0b0100000010100100;
    rom_controle[8][1] = 0b00;
    rom_controle[9][0] = 0b1000000100000000;
    rom_controle[9][1] = 0b00;

    /**
     * matriz emulando a memoria ROM com a tabela de despacho 1
     **/
    rom_despacho_1[0][0] = 0b000000;
    rom_despacho_1[0][1] = 0b0110;
    rom_despacho_1[1][0] = 0b000010;
    rom_despacho_1[1][1] = 0b1001;
    rom_despacho_1[2][0] = 0b000100;
    rom_despacho_1[2][1] = 0b1000;
    rom_despacho_1[3][0] = 0b100011;
    rom_despacho_1[3][1] = 0b0010;
    rom_despacho_1[4][0] = 0b101011;
    rom_despacho_1[4][1] = 0b0010;

    /**
     * matriz emulando a memoria ROM com a tabela de despacho 2
     **/
    rom_despacho_2[0][0] = 0b100011;
    rom_despacho_2[0][1] = 0b0011;
    rom_despacho_2[0][0] = 0b101011;
    rom_despacho_2[0][1] = 0b0101;

    static int estado;
    int CtlEnd;

    /**
     * função que retorna o próximo estado através da tabela de despacho 1
     **/
    int getEstadoDespacho1()
    {
        int i;
        char op;
        op = IR & separa_cop;

        for (i = 0; i < 5; i++) {
            if (rom_despacho_1[i][0] == op) return rom_despacho_1[i][1];
        }
        return 0;
    }

    /**
     * função que retorna o próximo estado através da tabela de despacho 2
     **/
    int getEstadoDespacho2()
    {
        int i;
        char op;
        op = IR & separa_cop;

        for (i = 0; i < 2; i++) {
            if (rom_despacho_2[i][0] == op) return rom_despacho_2[i][1];
        }
        return 0;
    }

    /**
     * caso o programa esteja sendo iniciado agora
     **/
    if (IR == -1) {
        estado = 0;
    /**
     * caso o IR tenha um valor válido
     **/
    } else {
        CtlEnd = rom_controle[estado][1]; // CtlEnd do estado atual
        switch (CtlEnd) {
            case 0:
                estado = 0;
                break;
            case 1:
                estado = getEstadoDespacho1();
                break;
            case 2:
                estado = getEstadoDespacho2();
                break;
            case 3:
                estado++;
                break;
        }
    }
    *sc = rom_controle[estado][0];
}

/**
 * Especificação:
 * Busca da Instrucao
 * void Busca_Instrucao(short int sc, int PC, int ALUOUT, int IR, int *PCnew, int *IRnew, int *MDRnew);
 * args de entrada: short int sc, int PC, int ALUOUT, int IR
 * args de saida: int *PCnew, int *IRnew, int *MDRnew
 **/
void Busca_Instrucao(int sc, int PC, int ALUOUT, int IR, int A, int B, int *PCnew, int *IRnew, int *MDRnew)
{
    /**
     * MemRead == 1 && ALUSrcA == 0 && IorD == 0 && IRWrite == 1 && ALUSrcB == 01
     * && ALUOp == 00 && PCWrite == 1 && PCSource == 00
     * exemplo:
     * sc                  = ***1 **** **** ****
     * separa_MemRead      = 0001 0000 0000 0000
     * sc | separa_MemRead = ***1 **** **** ****
     * sc == (sc | separa_MemRead) => MemRead = 1
     **/
    if (sc == (sc | separa_MemRead)
        && sc != (sc | separa_ALUSrcA)
        && sc != (sc | separa_IorD)
        && sc == (sc | separa_IRWrite)
        && sc != (sc | separa_ALUSrcB1)
        && sc == (sc | separa_ALUSrcB0)
        && sc != (sc | separa_ALUOp1)
        && sc != (sc | separa_ALUOp0)
        && sc == (sc | separa_PCWrite)
        && sc != (sc | separa_PCSource1)
        && sc != (sc | separa_PCSource0)
    ) {
        // condição de parada
        #ifdef DEBUG
        printf("%d", PC>>2);
        #endif
        if (memoria[PC>>2] == 0x0) {
            loop = 0;
        }
        *IRnew = memoria[PC>>2];
        *PCnew = PC + 4;
    }
}

/**
 * Especificação:
 * Decodifica Instrucao, Busca Registradores e Calcula Endereco para beq
 * void Decodifica_BuscaRegistrador(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew);
 * args de entrada: short int sc, int IR, int PC, int A, int B,
 * args de saida: int *Anew, int *Bnew, int *ALUOUTnew
 **
 * Comentários do aluno:
 * Se os sinais de controle estão OK lê rs e rt e calcula o endereço para beq, caso contrario
 * não efetua nenhuma ação.
 **/
void Decodifica_BuscaRegistrador(int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew)
{
    /**
     * Busca de registradores/decodificação de instruções
     * ALUSrcA == 0 && ALUSrcB == 11 && ALUOp == 00
     * exemplo:
     * sc               = **** **** **** *0**
     * separa_ALUSrcA   = 0000 0000 0000 0100
     * sc | separa_IorD = **** **** **** *1**
     * sc != (sc | separa_IorD) => ALUSrcA = 0
     **/
    if (sc != (sc | separa_ALUSrcA)
        && sc == (sc | separa_ALUSrcB1) && sc == (sc | separa_ALUSrcB0)
        && sc != (sc | separa_ALUOp1)   && sc != (sc | separa_ALUOp0)
    ) {
        /**
         * exemplo:
         * IR            = 000000 00010 01011 00010 00000 100000
         * separa_rs     = 000000 11111 00000 00000 00000 000000
         * IR & rs       = 000000 00010 00000 00000 00000 000000
         * (IR & rs)>>21 = 00010 = 2 = $v0
         **/
        *Anew = reg[(IR & separa_rs)>>21];
        /**
         * exemplo:
         * IR            = 000000 00010 01011 00010 00000 100000
         * separa_rt     = 000000 00000 11111 00000 00000 000000
         * IR & rs       = 000000 00000 01011 00000 00000 000000
         * (IR & rs)>>16 = 01011 = 11 = $t3
         **/
        *Bnew = reg[(IR & separa_rt)>>16];
        /**
         * exemplo:
         * IR                        = 000000 00010 01011 00010 00000 100000
         * separa_imediato           = 000000 00000 00000 11111 11111 111111
         * IR & separa_imediato      = 000000 00000 00000 00010 00000 100000
         * (IR & separa_imediato)<<2 = 000000 00000 00000 01000 00010 000000
         **/
        *ALUOUTnew = PC + ((IR & separa_imediato)<<2);
    }
}

/**
 * Especificação:
 * Executa TipoR, Calcula endereco para lw/sw e efetiva desvio condicional e incondicional
 * void Execucao_CalcEnd_Desvio(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew);
 * args de entrada: short int sc, int A, int B, int IR, int PC, int ALUOUT
 * args de saida: int *ALUOUTnew, int *PCnew
 **/
void Execucao_CalcEnd_Desvio(int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
    /**
     * lw ou sw
     * ALUSrcA == 1 && ALUSrcB == 10 && ALUOp == 00
     **/
    if (sc == (sc | separa_ALUSrcA)
        && sc == (sc | separa_ALUSrcB1) && sc != (sc | separa_ALUSrcB0)
        && sc != (sc | separa_ALUOp1)   && sc != (sc | separa_ALUOp0)
    ) {
        *ALUOUTnew = A + (IR & separa_imediato);

    /**
     * tipo r
     * ALUSrcA == 1 && ALUSrcB == 00 && ALUOp == 10
     **/
    } else if (sc == (sc | separa_ALUSrcA)
        && sc != (sc | separa_ALUSrcB1) && sc != (sc | separa_ALUSrcB0)
        && sc == (sc | separa_ALUOp1)   && sc != (sc | separa_ALUOp0)
    ) {
        /**
         * campo de função -> ula_op
         * exemplo:
         * IR                  = 000000 00000 00000 00000 00000 100000
         * separa_cfuncao      = 000000 00000 00000 00000 00000 111111
         * IR & separa_cfuncao = 000000 00000 00000 00000 00000 100000
         **/
        char ula_op, zero, overflow;
        switch (IR & separa_cfuncao) {
            case 0x24:
                ula_op = 0x00; // and
                break;
            case 0x25:
                ula_op = 0x01; // or
                break;
            case 0x20:
                ula_op = 0x02; // add
                break;
            case 0x22:
                ula_op = 0x02; // sub
                break;
            case 0x2a:
                ula_op = 0x02; // slt
                break;
        }
        ula(A, B, ula_op, ALUOUTnew, &zero, &overflow);

    /**
     * beq
     * ALUSrcA == 1 && ALUSrcB == 00 && ALUOp == 01 && PCWriteCond == 1 && PCSource == 01
     */
    } else if (sc == (sc | separa_ALUSrcA)
        && sc != (sc | separa_ALUSrcB1)  && sc != (sc | separa_ALUSrcB0)
        && sc == (sc | separa_ALUOp1)    && sc != (sc | separa_ALUOp0)
        && sc != (sc | separa_PCSource1) && sc == (sc | separa_PCSource0)
    ) {

    /**
     * jump
     * PCWrite = 1 && PCSource = 10
     */
    } else if (sc == (sc | separa_PCWrite)
        && sc == (sc | separa_PCSource1) && sc != (sc | separa_PCSource0)
    ) {
        /**
         * exemplo:
         * PC                             = 1001** ***** ***** ***** ***** ******
         * separa_4bits_PC                = 111100 00000 00000 00000 00000 000000
         * PC & separa_4bits_PC           = 100100 00000 00000 00000 00000 000000 = temp1
         * IR                             = ****** 10011 11111 00000 00011 100111
         * separa_endereco_jump           = 000000 11111 11111 11111 11111 111111
         * IR & separa_endereco_jump      = 000000 10011 11111 00000 00011 100111
         * (IR & separa_endereco_jump)<<2 = 000010 01111 11100 00000 01110 011100 = temp2
         * temp1 | temp 2                 = 100110 01111 11100 00000 01110 011100
         **/
        *PCnew = (PC & separa_4bits_PC) | (IR & separa_endereco_jump)<<2;
    }
}

/**
 * Especificação:
 * Escreve no Bco de Regs resultado TiporR, Le memoria em lw e escreve na memoria em sw
 * void EscreveTipoR_AcessaMemoria(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew);
 * args de entrada: short int sc, int B, int IR, int ALUOUT, int PC
 * args de saida: int *MDRnew, int *IRnew
 **/
void EscreveTipoR_AcessaMemoria(int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew)
{
    /**
     * lw
     * MemRead == 1 && IorD == 1
     **/
    if (sc == (sc | separa_MemRead) && sc == (sc | separa_IorD)) {
        *MDRnew = memoria[ALUOUT];

    /**
     * sw
     * MemWrite == 1 && IorD == 1
     **/
    } else if (sc == (sc | separa_MemWrite) && sc == (sc | separa_IorD)) {
        memoria[ALUOUT] = B;

    /**
     * tipo r
     * RegDst == 1 && RegWrite == 1 && MemtoReg == 0
     **/
    } else if (sc == (sc | separa_RegDst) && sc == (sc | separa_RegWrite) && sc != (sc | separa_MemtoReg)) {
        int tmp = (IR & separa_cfuncao);
        reg[(IR & separa_cfuncao)] = ALUOUT;
    }
}

/**
 * Especificação:
 * Escreve no Bco de Regs o resultado da leitura da memoria feita por lw
 * void EscreveRefMem(short int sc, int IR, int MDR, int ALUOUT);
 * args de entrada: short int sc, int IR, int MDR, int ALUOUT
 * args de saida: nao ha
 **/
void EscreveRefMem(int sc, int IR, int MDR, int ALUOUT)
{
    /**
     * lw
     * RegDst == 1 && RegWrite == 1 && MemtoReg == 0
     **/
     if (sc == (sc | separa_RegDst) && sc == (sc | separa_RegWrite) && sc != (sc | separa_MemtoReg)) {
        reg[(IR & separa_rt)>>16] = MDR;
     }
}

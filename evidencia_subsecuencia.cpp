//Sofía Moreno López A01028251
//Omar Emiliano Sánchez Villegas A01749975
//Nicole Dávila Hernández A01784217

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//O(n)
string leerArchivo(const string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string contenido;
    string linea;
    while (getline(archivo, linea)) {
        contenido += linea;
    }
    return contenido;
}

//O(n)
vector<int> funcionZ(const string str) {
    int n = str.length();
    vector<int> z(n);
    for (int i = 1, izq = 0, derecha = 0; i < n; i++) {
        if (i <= derecha)
            z[i] = min(derecha - i + 1, z[i - izq]);
        while (i + z[i] < n && str[z[i]] == str[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > derecha)
            izq = i, derecha = i + z[i] - 1;
    }
    return z;
}

//O(n*m)
pair<bool, int> buscarPatronSubsecuencia(const string texto, const string patron) {
    if (patron.empty()) return {false, -1};
    if (texto.empty()) return {false, -1};
    
    int n = texto.length();
    int m = patron.length();
    int j = 0;
    
    for (int i = 0; i < n && j < m; i++) {
        if (texto[i] == patron[j]) {
            if (j == 0) {
                pair<bool, int> posicionInicio = {true, i + 1};

                int tempI = i;
                int tempJ = j;
                
                while (tempI < n && tempJ < m) {
                    if (texto[tempI] == patron[tempJ]) {
                        tempJ++;
                        if (tempJ == m) {
                            return posicionInicio;
                        }
                    }
                    tempI++;
                }
            }
            j++;
        }
    }
    return {false, -1};
}

//O(n + m)
pair<bool, int> buscarPatronSubstr(const string texto, const string patron) {
    if (patron.empty() || texto.empty()) return {false, -1};
    
    string concat = patron + "#" + texto;
    vector<int> z = funcionZ(concat);
    for (int i = patron.length() + 1; i < concat.length(); i++) {
        if (z[i] == patron.length()) {
            return {true, i - patron.length()}; 
        }
    }
    return {false, -1};
}

//O(n^2)
pair<int, int> palindromoMasLargo(const string str) {
    if (str.empty()) return {0, 0};
    
    string s = "#";
    for (char centro : str) {
        s += centro;
        s += "#";
    }
    
    int n = s.length();
    vector<int> p(n, 0);  
    int centro = 0; 
    int derecha = 0;
    int lenMax = 0;
    int centroMax = 0;
    
    for (int i = 0; i < n; i++) {
        if (i < derecha) {
            int mirror = 2 * centro - i;
            p[i] = min(derecha - i, p[mirror]);
        }

        int izquierda = i - (p[i] + 1);
        int derecha = i + (p[i] + 1);
        while (izquierda >= 0 && derecha < n && s[izquierda] == s[derecha]) {
            p[i]++;
            izquierda--;
            derecha++;
        }

        if (i + p[i] > derecha) {
            centro = i;
            derecha = i + p[i];
        }
        
        if (p[i] > lenMax) {
            lenMax = p[i];
            centroMax = i;
        }
    }

    int inicio = (centroMax - lenMax) / 2;
    int len = lenMax;
    
    return {inicio + 1, inicio + len};
}

//O(n*m)
pair<int, int> substrComunMasLarga(const string str1, const string str2) {
    if (str1.empty() || str2.empty()) return {0, 0};
    
    vector<vector<int>> dp(str1.length() + 1, vector<int>(str2.length() + 1, 0));
    int lenMax = 0;
    int posicionFinal = 0;
    
    for (int i = 1; i <= str1.length(); i++) {
        for (int j = 1; j <= str2.length(); j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                if (dp[i][j] > lenMax) {
                    lenMax = dp[i][j];
                    posicionFinal = i;
                }
            }
        }
    }
    
    return {posicionFinal - lenMax + 1, posicionFinal};
}

int main() {
    string transmision1 = leerArchivo("transmission1.txt");
    string transmision2 = leerArchivo("transmission2.txt");
    string codigoM1 = leerArchivo("mcode1.txt");
    string codigoM2 = leerArchivo("mcode2.txt");
    string codigoM3 = leerArchivo("mcode3.txt");
    
    // Parte 1: Búsqueda de patrones
    vector<pair<string, string>> busquedas = {
        {transmision1, codigoM1}, {transmision1, codigoM2}, {transmision1, codigoM3},
        {transmision2, codigoM1}, {transmision2, codigoM2}, {transmision2, codigoM3}
    };
    
    for (const pair<string, string> busqueda : busquedas) {
        pair<bool, int> resultado = buscarPatronSubstr(busqueda.first, busqueda.second);
        if (resultado.first) {
            cout << "true " << resultado.second << endl;
        } else {
            cout << "false" << endl;
        }
    }
    
    // Parte 2: Búsqueda de palíndromos
    pair<int, int> palindromo1 = palindromoMasLargo(transmision1);
    pair<int, int> palindromo2 = palindromoMasLargo(transmision2);
    
    cout << palindromo1.first << " " << palindromo1.second << endl;
    cout << palindromo2.first << " " << palindromo2.second << endl;
    
    // Parte 3: Substr común más larga
    pair<int, int> substrComun = substrComunMasLarga(transmision1, transmision2);
    cout << substrComun.first << " " << substrComun.second << endl;
    
    return 0;
}
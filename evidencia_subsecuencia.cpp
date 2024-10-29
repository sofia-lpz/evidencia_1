//Sofia Moreno Lopez A01028251
//Omar Emiliano Sanchez Villegas A01749975
//Nicole Davila Hernandez A01784217

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//O(n)
string leeArchivo(const string nombreArchivo) {
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
    for (int i = 1, l = 0, derecha = 0; i < n; i++) {
        if (i <= derecha)
            z[i] = min(derecha - i + 1, z[i - l]);
        while (i + z[i] < n && str[z[i]] == str[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > derecha)
            l = i, derecha = i + z[i] - 1;
    }
    return z;
}

//O(n*m)
pair<bool, int> buscaPatronSubsecuencia(const string& texto, const string& patron) {
    if (patron.empty()) return {false, -1};
    if (texto.empty()) return {false, -1};
    
    int n = texto.length();
    int m = patron.length();
    int j = 0;
    
    for (int i = 0; i < n && j < m; i++) {
        if (texto[i] == patron[j]) {
            if (j == 0) {
                pair<bool, int> startPos = {true, i + 1}; // Convert to 1-based index

                int tempI = i;
                int tempJ = j;
                
                while (tempI < n && tempJ < m) {
                    if (texto[tempI] == patron[tempJ]) {
                        tempJ++;
                        if (tempJ == m) {
                            return startPos;
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
pair<bool, int> buscaPatronSubstring(const string texto, const string patron) {
    if (patron.empty() || texto.empty()) return {false, -1};
    
    string concat = patron + "#" + texto;  // O(n + m)
    vector<int> z = funcionZ(concat);      // O(n + m)
    
    for (int i = patron.length() + 1; i < concat.length(); i++) {
        if (z[i] == patron.length()) {
            return {true, i - patron.length()}; 
        }
    }
    return {false, -1};
}

//O(n^2)
pair<int, int> palindromoMasLargo(const string& str) {
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
    int maxLen = 0;
    int maxCentro = 0;
    
    for (int i = 0; i < n; i++) {
        if (i < derecha) {
            int mirror = 2 * centro - i;
            p[i] = min(derecha - i, p[mirror]);
        }

        int left = i - (p[i] + 1);
        int right = i + (p[i] + 1);
        while (left >= 0 && right < n && s[left] == s[right]) {
            p[i]++;
            left--;
            right++;
        }

        if (i + p[i] > derecha) {
            centro = i;
            derecha = i + p[i];
        }
        
        if (p[i] > maxLen) {
            maxLen = p[i];
            maxCentro = i;
        }
    }

    int start = (maxCentro - maxLen) / 2;
    int length = maxLen;
    
    return {start + 1, start + length};
}

//O(n*m)
pair<int, int> substringComunMasLargo(const string str1, const string str2) {
    if (str1.empty() || str2.empty()) return {0, 0};
    
    vector<vector<int>> dp(str1.length() + 1, vector<int>(str2.length() + 1, 0));
    int longitudMax = 0;
    int posicionFinal = 0;
    
    for (int i = 1; i <= str1.length(); i++) {
        for (int j = 1; j <= str2.length(); j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                if (dp[i][j] > longitudMax) {
                    longitudMax = dp[i][j];
                    posicionFinal = i;
                }
            }
        }
    }
    
    return {posicionFinal - longitudMax + 1, posicionFinal};
}

int main() {
    string transmision1 = leeArchivo("transmission1.txt");
    string transmision2 = leeArchivo("transmission2.txt");
    string mcode1 = leeArchivo("mcode1.txt");
    string mcode2 = leeArchivo("mcode2.txt");
    string mcode3 = leeArchivo("mcode3.txt");
    
    // Parte 1: Busqueda de patrones
    vector<pair<string, string>> busquedas = {
        {transmision1, mcode1}, {transmision1, mcode2}, {transmision1, mcode3},
        {transmision2, mcode1}, {transmision2, mcode2}, {transmision2, mcode3}
    };
    
    for (const pair<string, string> busqueda : busquedas) {
        pair<bool, int> resultado = buscaPatronSubsecuencia(busqueda.first, busqueda.second);
        if (resultado.first) {
            cout << "true " << resultado.second << endl;
        } else {
            cout << "false" << endl;
        }
    }
    
    // Parte 2: Busqueda de palindromos
    pair<int, int> palindromo1 = palindromoMasLargo(transmision1);
    pair<int, int> palindromo2 = palindromoMasLargo(transmision2);
    
    cout << palindromo1.first << " " << palindromo1.second << endl;
    cout << palindromo2.first << " " << palindromo2.second << endl;
    
    // Parte 3: Substring comun mas largo
    pair<int, int> substringComun = substringComunMasLargo(transmision1, transmision2);
    cout << substringComun.first << " " << substringComun.second << endl;
    
    return 0;
}
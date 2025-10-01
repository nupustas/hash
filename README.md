# Hash funkcijos interpretacija
----
## 'Hashas' funkcijos aprašymas pseudo-kodu

### Pradinės reikšmės:
```
Sukurti state[4] masyvą su pradinėmis reikšmėmis, hasho 'atminčiai':
    state[0] = 0xabcdef1234567890
    state[1] = 0xfedcba9876543210  
    state[2] = 0xa12b34c56d78e90f
    state[3] = 0xa1b2c3d4e5f60708
```

### 1 ETAPAS - Simbolių apdorojimas:
```
Kiekvienam simboliui įvestyje:
    c = simbolio ASCII kodas (0-255)
    pozicija = simbolio numeris mod 4
    
    state[pozicija] XOR= (c * 0x0101010101010101)
    
    kita_pozicija = (pozicija + 1) mod 4
    shift_kiekis = simbolio_numeris mod 56
    state[kita_pozicija] += (c << shift_kiekis)
```

### 2 ETAPAS - Tarpusavio maišymas:
```
Kiekvienam i nuo 0 iki 3:
    // Cirkuliarinis maišymas (0->1->2->3->0)
    state[i] XOR= state[(i+1) mod 4]
    
    state[i] *= 3
```

### 3 ETAPAS - Diagonalinis maišymas:
```
Kiekvienam i nuo 0 iki 3:
    // Diagonalinis maišymas (0->2, 1->3)
    state[i] += state[(i+2) mod 4]
    
    // Aukštųjų ir žemųjų bitų maišymas
    state[i] XOR= (state[i] >> 16)
```

### 4 ETAPAS - Konvertavimas į hex string'ą:
```
Kiekvienam state masyvo elementui:
    Konvertuoti į 16 simbolių hex string'ą
    Pridėti prie rezultato

Grąžinti 64 simbolių hex string'ą
```







## Efektyvumo testas naudojant 'Konstitucija.txt'
| Eil. sk. | Laikas (s) |
| -------- | ------- | 
| 8        | 0.00034 |
| 16       | 0.00036 |
| 32       | 0.00075 | 
| 64       | 0.00059 |
| 128      | 0.00098 |
| 256      | 0.00214 | 
| 512      | 0.00322 | 
| 789      | 0.00936 | 
# Logika za zadatak

Prvo proveravam da li je svaki deo manji od onog glavnog dela
To podrazumeva proveru da li je necija sirina veca od glavnog dela, isto vazi i za visinu,
takodje proveravam da li je povrsina svih delova <= povrsini glvano dela


Imam 2 ideje za ovaj zadatak:

Prvo sve pravougaonike pamtimo u memoriji i soritramo ih u listu.

https://chatgpt.com/share/677554da-eb08-8004-8d67-c5df3c5f34b9

prvo BEZ rotiranja

Prvo sortiramo 'rect'-ove od najveceg do najmanjeg. Kada to uradimo ako 'rect' NIJE kvadrat moramo da uracunamo smestanje VODORAVNO i USPRAVNO.
Tada dobijemo 2 nove pozicije (desno od postavljenog 'rect'-a i dole od postavljenog 'rect'-a) i ciklus se ponavlja.



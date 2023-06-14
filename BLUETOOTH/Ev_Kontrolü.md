Kullanılan Malzemeler;
-STM32F103RB
-HC06 Bluetooth Modülü
-ULN2003A Entegresi
-28BYJ-48 Step Motor

Atollic TrueStudio üzerinden geliştirilen projede bilgisayarın bluetooth'u üzerinden veri göndermek için arduino terminali kullanılmıştır.
Projenin ilk aşamasında gelen ve gönderilecek veriyi tutacak olan değişkenler, Clock ayarlamaları,delay fonksiyonu ayarlamaları vs. gerçekleştirilmiştir.
Daha sonrasında, RX ve TX olarak kullanılacak pinler,ledlerin ve motorun pinleri tanımlanmıştır. Haberleşme için gerekli olan usart
ayarlamaları baud rate 9600 ,RX ve TX mod,8 bit veri uzunluğu olacak şekilde ve veri geldiğinde kullanılacak olan interrup bayrağı
tanımlanmıştır.Daha sonrasında usart aktif edilmiştir. NVIC üzerinden hangi interruptın kullanılacağı ve öncelik ayarlamaları gerçekleştirilmiştir.
Usart1 interrupt'ı içerisine herhangi bir veri geldiğinde kayıt altına alınacak olan buffera verinin taşınması işlemi yapılmıştır.
Veri aktarımı sonlandığında dizi başa döndürülmüştür.Gönderim aşamasında ise SR registerı göz önünde bulundurularak string pointerının
işaret ettiği yerdeki veriler sırası ile gönderilmektedir.Bluetooth üzerinden gelen veri koşullar ile karşılaştırılır ve uygun koşul numarası 
kosul değişkenine aktarılıp evkontrol veya garajayar çağırılır.Bu çağırma işlemi her tamamlandığında koşul tekrar sıfırlanır.Garajayar
çağırımı bittikten sonra ise motoru kontrol eden pinler hareketin kesilmesi için resetlenir. Ev kontrolünde uygun pinler set veya reset edilir
işlemin yapıldığına dair bilgi modül üzerinden terminale geri döndürülür ve bufferlar daha sonra boşaltılır.Garaj ayarında ise istenilen açı
ilk olarak 1 adımın yaptığı açıya bölünür ve kaç adım atması gerektiği garaj kontrolün kaç kere tekrar edeceği belirtilir.1 adım için 4 pinin
ayrı ayrı sırasıyla çalışması gerektiği için bu for döngüsünün içinde hepsini 1 kere çalıştıracak bir döngü tekrar yazılır.Hız ayarlanabilir şekildedir.

Projenin videosu ve fotoğraflarına aşağıda ulaşabilirsiniz.


![genel](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/cc0154bd-290a-49f8-ab05-31abe6bc6a49)
![modul](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/3478ac0a-2ca1-4d83-88aa-fb7dd0f30689)
![terminal](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/c3516a74-772e-440c-a510-1b1789a7b515)

https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/6fab5b35-1fb0-485d-a776-faa45b901a4b


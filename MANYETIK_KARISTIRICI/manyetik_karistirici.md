Geliştirilen IDE: STM32 CubeIDE
Kırmızı LED: Manyetik karıştırıcıyı döndüren DC motoru temsil etmektedir.
Yeşil LED: Makinenin Meşgul olduğu durum boyunca yanmaktadır.
Buton: Start butonu olarak kullanılmıştır.
Ekran: Motor hız değerini,durum animasyonlarını izlemek için kullanılmıştır.

AÇIKLAMA: Proje ilk başlatıldığında bir süre MERHABA animasyonu bizi karşılamaktadır. Daha sonrasında ayar ekranı geldiğinde
potansiyometre ile manyetik karıştırıcının % cinsinden hız değeri ayarlanabilmektedir. Ayar yapıldıktan sonra start butonuna
basıldığı anda meşgul durumunu bildiren yeşil led aktif olmaktadır ve manyetik motorunu temsil eden kırmızı led aktif duruma 
gelmektedir.Aynı anda işlemin yapıldığını belirten animasyon OLED ekranda oynatılmaktadır.
Belirlenen süre sonunda ilk olarak manyetik motor durmakta daha sonrasında işlemin bittiğini belirten animasyon tamamlanınca
meşgul durumunu belirten led sönmektedir.Sistem tekrar manyetik karıştırıcı hız değerini istemekte ve kullanıma hazır hale 
gelmektedir.

Aşağıda düşük ve yüksek hızda fotoğraflar ve tam simülasyonu gösteren video bulunmaktadır.

https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/d34f5d89-b756-495d-b863-db38a9c6db4c
![dusuk_ekran](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/3af0613a-a970-46af-9ad3-a88eb8f2e314)
![dusuk_led](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/74fd7d4b-365f-49dd-b596-16910bba070e)

![yuksek_led](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/dcd4c139-b38f-4013-92ac-f41fdf0cea92)
![yuksek_ekran](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/52b9d226-bcf2-472e-bcc0-03ce58e407e2)






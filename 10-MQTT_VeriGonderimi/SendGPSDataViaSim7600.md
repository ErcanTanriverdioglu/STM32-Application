Çağırılan MQTT_Protocol_Settings Fonksiyonu içerisinde herhangi bir çakışmanın önlenmesi için ilk olarak disconnect ve stop komutları
SIMVeriGonder fonksiyonu aracılığı ile gönderilir.Daha sonra AT ve CREG komutları gönderilerek geri dönen mesaj kontrol edilir.Bilgi
olarak Buffera OK dönüşü sağlandıktan sonra uygun değişkenler 1 yapılır. ATReady ve CREGReady 1 olduktan sonra GPS anteni için gerekli
olan AT komutları ve MQTT bağlantısı için gerekli olan AT komutları sırası ile çalıştırılır. Herhangi bir aşamada hata olup olmadığını
kontrol etmek için ise SIMVeriGonder içerisine SimError değişkeni tanımlanmıştır.Eğer CREGReady olmamış ise döngü içerisinde sürekli
olarak çağırılır.Daha sonrasında GPS verisi okunması AT komutları aracılığı ile yapılır ve eğer veri içerisinde "+GPSINFO:" var ise
Buffer içerisindeki geri dönen veriler GPSBuffer içerisine atılır. Gelen veriler sırası ile ve virgülle ayrıldığı için bu verileri
SIM_GPS_Edit fonksiyonu çağırılarak RawData isminde oluşturulan matris içerisine daha sonrasında ise uygun değişkenlere atılır.
Bu işlemler bittikten sonra SetData çağırılarak gönderimi istenen değerler DataBuffer'a yüklenir.Yükleme işlemi bittikten sonra
MQTT_Protocol_Send çağırılarak ilk önce karşı tarafın takip edeceği kuyruk ismi daha sonrasında ise Bufferdaki veriler gönderilir.
AT+CMQTTPUB komutu kullanılarak bu veriler uzak sunucuya Publish edilir.

Aşağıda debug ekranı üzerinden ve uzak sunucu üzerinden veri takibine ilişkin görüntüler görülebilmektedir.

![toplugps](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/63c7eabe-97be-451f-8026-c3e005402947)


![gpsmqtt](https://github.com/ErcanTanriverdioglu/STM32_Uygulamalar/assets/122203202/3283bc11-ce12-4731-960e-7e4b234dc500)

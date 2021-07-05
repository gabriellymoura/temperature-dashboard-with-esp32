setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      y = parseFloat(this.responseText);
      document.getElementById("temp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

function atualizaRelogio(){ 
  var momentoAtual = new Date();
  
  var vhora = momentoAtual.getHours();
  var vminuto = momentoAtual.getMinutes();
  var vsegundo = momentoAtual.getSeconds();
  
  var vdia = momentoAtual.getDate();
  var vmes = momentoAtual.getMonth() + 1;
  var vano = momentoAtual.getFullYear();
  
  if (vdia < 10){ vdia = "0" + vdia;}
  if (vmes < 10){ vmes = "0" + vmes;}
  if (vhora < 10){ vhora = "0" + vhora;}
  if (vminuto < 10){ vminuto = "0" + vminuto;}
  if (vsegundo < 10){ vsegundo = "0" + vsegundo;}

  dataFormat = vdia + " de " + vmes + " de " + vano;
  horaFormat = vhora + " : " + vminuto + " : " + vsegundo;

  document.getElementById("data").innerHTML = dataFormat;
  document.getElementById("hora").innerHTML = horaFormat;

  setTimeout("atualizaRelogio()",1000);
}

var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  title: { text: 'Temperatura do Asfalto' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#39A0ED' }
  },
  xAxis: { type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Temperatura (Celsius)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/temperaturetwo", true);
  xhttp.send();
}, 1000 ) ;
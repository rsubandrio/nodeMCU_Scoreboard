#include "Arduino.h"

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <title>IoT Scoreboard</title>
  <style>
    html {
      font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
      display: inline-block;
      text-align: center;
      width: 100%;
    }

    table {
      width: 100%;
    }

    td {
      text-align: center;
    }

    footer {
      margin-top: 2rem;
    }

    #score_one,
    #score_two {
      background-color: #d2dd9e;
      color: rgb(226, 43, 113);
      text-align: center;
      vertical-align: middle;
      font-size: 10rem;
    }

    .btn {
      width: 100%;
      font-size: 1rem;
    }
  </style>
</head>

<body>
  <h1>Scoreboard</h1>
  <table>
    <tr>
      <td colspan="3">
        <div id="score_one" class="btn">00</div>
      </td>
      <td colspan="3">
        <div id="score_two" class="btn">00</div>
      </td>
    </tr>
    <tr>
      <td><button id="btn_add_score_one" class="btn">+</button></td>
      <td><button id="btn_min_score_one" class="btn">-</button></td>
      <td><button id="btn_rst_score_one" class="btn">~</button></td>
      <td><button id="btn_rst_score_two" class="btn">~</button></td>
      <td><button id="btn_min_score_two" class="btn">-</button></td>
      <td><button id="btn_add_score_two" class="btn">+</button></td>
    </tr>
  </table>
  <footer>
    <!-- Tolong ini jangan dihapus -->
    <!-- Please don't remove these lines -->
    <p>
      By: <a href="https://github.com/rsubandrio" target="_blank" rel="noopener noreferrer">Rio Subandrio</a>
    </p>
    <!-- Tolong ini jangan dihapus -->
    <!-- Please don't remove these lines -->
  </footer>

  <script>
    let point_one = 0;
    let point_two = 0;
    let score_one = document.getElementById("score_one");
    let score_two = document.getElementById("score_two");
    let btn_add_score_one = document.getElementById("btn_add_score_one");
    let btn_min_score_one = document.getElementById("btn_min_score_one");
    let btn_rst_score_one = document.getElementById("btn_rst_score_one");
    let btn_rst_score_two = document.getElementById("btn_rst_score_two");
    let btn_min_score_two = document.getElementById("btn_min_score_two");
    let btn_add_score_two = document.getElementById("btn_add_score_two");

    score_one.addEventListener("click", function () {
      point_one++;
      update_score();
    });

    btn_add_score_one.addEventListener("click", function () {
      point_one++;
      update_score();
    });

    btn_min_score_one.addEventListener('click', function () {
      point_one--;
      update_score();
    });

    btn_rst_score_one.addEventListener("click", function () {
      point_one = 0;
      update_score();
    });

    score_two.addEventListener("click", function () {
      point_two++;
      update_score();
    });

    btn_add_score_two.addEventListener("click", function () {
      point_two++;
      update_score();
    });

    btn_min_score_two.addEventListener('click', function () {
      point_two--;
      update_score();
    });

    btn_rst_score_two.addEventListener("click", function () {
      point_two = 0;
      update_score();
    });

    function update_score() {
      if (point_one > 30 || point_one < 1) {
        point_one = 0;
      }
      if (point_two > 30 || point_two < 1) {
        point_two = 0;
      }
      let url = "/update?";
      let display_score_one = point_one < 10 ? "0" + point_one : point_one;
      let display_score_two = point_two < 10 ? "0" + point_two : point_two;
      score_one.innerHTML = display_score_one;
      score_two.innerHTML = display_score_two;
      url += "l=" + display_score_one + "&r=" + display_score_two;
      let gameover = false;
      if (point_one == 21 && point_two < 20) gameover = true;
      if (point_two == 21 && point_one < 20) gameover = true;
      if (point_one >= 20 && point_two >= 20 && Math.abs(point_one - point_two) == 2) gameover = true;
      if (point_one == 30 || point_two == 30) gameover = true;

      if (gameover == true) {
        url += "&gameover=1";
        point_one = point_two = 0;
        update_score();
      }

      let request = new XMLHttpRequest();
      request.open("GET", url, true);
      request.onload = function () {
        if (this.status >= 200 && this.status < 400) {
          console.log("Berhasil! " + this.response);
        } else {
          console.log("Sampai tapi error " + this.response);
        }
      };

      request.onerror = function () {
        console.log("Connection Error!!");
      };

      request.send();
    }
  </script>
</body>

</html>)rawliteral";

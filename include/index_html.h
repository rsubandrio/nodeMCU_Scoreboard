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

    div.btn {
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
        <div id="display_l" class="btn" onclick="p('l')">00</div>
      </td>
      <td colspan="3">
        <div id="display_r" class="btn" onclick="p('r')">00</div>
      </td>
    </tr>
    <tr>
      <td><button class="btn" onclick="p('l')">+</button></td>
      <td><button class="btn" onclick="m('l')">-</button></td>
      <td><button class="btn" onclick="r('l')">~</button></td>
      <td><button class="btn" onclick="r('r')">~</button></td>
      <td><button class="btn" onclick="m('r')">-</button></td>
      <td><button class="btn" onclick="p('r')">+</button></td>
    </tr>
    <tr>
      <td colspan="6"><div id="status">Status: </div></td>
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
    let score_l = 0;
    let score_r = 0;
    let prev_scorer = false;
    let is_point = false;
    let display_l = document.getElementById('display_l');
    let display_r = document.getElementById('display_r');
    let buttons = document.getElementsByClassName('btn');

    function p(side) {
      side == 'l' ? score_l++ : score_r++;
      prev_scorer !== side ? is_point = false : is_point = true;
      prev_scorer = side;
      update();
    }

    function m(side) {
      side == 'l' ? score_l-- : score_r--;
      update();
    }

    function r(side) {
      side == 'l' ? score_l = 0 : score_r = 0;
      is_point = "Whack";
      prev_scorer = false;
      update();
    }

    function update()
    {
      score_l <= 0 || score_l > 30 ? score_l = 0 : score_l;
      score_r <= 0 || score_r > 30 ? score_r = 0 : score_r;

      let gameover = ((score_l == 21) && (score_r < 20)) || ((score_r == 21) && (score_l < 20)) || ((score_l >= 20) && (score_r >= 20) && (Math.abs(score_l - score_r) == 2)) || ((score_l == 30) || (score_r == 30));
      let disp_score_l = score_l < 10 ? '0' + score_l : score_l;
      let disp_score_r = score_r < 10 ? '0' + score_r : score_r;
      let url = '/update?';
      url += 'l=' + disp_score_l + '&r=' + disp_score_r + '&s=' + prev_scorer + '&ip=' + is_point;
      if (gameover) {
        url += '&gameover=1';
        disableBtn(true);
        setTimeout(function() {
          score_l = score_r = 0;
          update();
          disableBtn(false);
        }, 3000);
      }
      display_l.innerText = disp_score_l;
      display_r.innerText = disp_score_r;
      url += '&buster='+new Date().getTime();
      sendRequest(url);
    }

    function sendRequest(url) {
      let status_bar = document.getElementById('status');
      let request = new XMLHttpRequest();
      request.open('GET', url, true);
      request.onload = function() {
        if (this.status >= 200 && this.status < 400) {
          resp = JSON.parse(this.response);
          //JSON.parse()
          status_bar.innerHTML = 'Status: Berhasil - respon server: ' + resp.status + " #" + resp.number;
        } else {
          status_bar.innerHTML = 'Status: Error - respon server: ' + this.response;
        }
      }
      request.onerror = function() {
        status_bar.innerHTML = 'Status: Error - Tidak ada respon dari server!';
      }
      request.send();
    }

    function disableBtn(bool) {
      for (let i = 0; i < buttons.length; i++) {
        buttons[i].disabled = bool;
      }
    }
  </script>
</body>

</html>)rawliteral";

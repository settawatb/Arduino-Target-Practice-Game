//  Arduino Target Practice Game
//  6351000457 Settawat Buddhakanchana
//  GoogleApp Script SOURCE CODE / JavaScript

function doGet(e) { //--> Function Script ในการเพิ่มค่าเข้าไปใน Spreadsheet
  Logger.log(JSON.stringify(e));
  var result = "Ok";
  if (e.parameter == "undefined") {
    result = "No Parameters";
  } else {
    var sheet_id = "XXXXX"; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Bangkok", "HH:mm:ss");
    rowData[1] = Curr_Time; // Time in column B
    for (var param in e.parameter) {
      Logger.log("In for loop, param=" + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ":" + e.parameter[param]);
      switch (param) {
        case "score":
          rowData[2] = value;
          result = "Score Written on column C";
          break;
      }
    }
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  Linenoti2(); //--> ส่งค่าไปที่ Line notify
  return ContentService.createTextOutput(result);
}

function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}

function Linenoti2() {
  //function Line notify
  //======================== ดึงข้อมูลจาก Spreadsheet ในแถวและตำแหน่งที่ต้องการแสดงผล
  var data_no1 = SpreadsheetApp.getActive()
    .getSheetByName("ชีต1") //--> ชื่อของ sheets
    .getRange(2, 3) //--> แถวและ column ของข้อมูล
    .getDisplayValues(); //--> ดึงค่าเก็บไว้ในตัวแปร data_no1
  var data_no2 = SpreadsheetApp.getActive()
    .getSheetByName("ชีต1")
    .getRange(3, 3)
    .getDisplayValues();
  var data_no3 = SpreadsheetApp.getActive()
    .getSheetByName("ชีต1")
    .getRange(4, 3)
    .getDisplayValues();
  var data_current = SpreadsheetApp.getActive()
    .getSheetByName("ชีต1")
    .getRange(11, 1)
    .getDisplayValues();

  //======================== ดึงข้อมูลวันที่และเวลาปัจจุบัน
  var d = new Date();
  var date =
    d.getDate() +
    "-" +
    (d.getMonth() + 1) +
    "-" +
    d.getFullYear() +
    " " +
    d.getHours() +
    ":" +
    d.getMinutes();

  //======================= เงื่อนไขถ้าหากว่าข้อมูลในตารางว่าง ให้เปลี่ยนค่าใน cell นั้น = 0
  if (data_no1 == "#NUM!") data_no1 = "0";
  if (data_no2 == "#NUM!") data_no2 = "0";
  if (data_no3 == "#NUM!") data_no3 = "0";
  if (data_current == "#N/A") data_current = "0";

  //--> ส่งค่าเพื่อไปแจ้งเตือนผ่าน Line Notification
  var token = "XXXXXXXXXXXXXXX"; //--> Line Notify Token
  var msg = //ข้อความที่ต้องการให้ Line แสดงผล
    "\n\n✨ARDUINO TARGET PRACTICE GAME✨ \n⭐️⭐️⭐️SCORE BOARD ⭐️⭐️⭐️\n" +
    "🥇NO.1 : " +
    data_no1 +
    " point\n" +
    "🥈NO.2 : " +
    data_no2 +
    " point\n" +
    "🥉NO.3 : " +
    data_no3 +
    " point\n" +
    "⚡️CURRENT SCORE⚡️ : " +
    data_current +
    " point\n\n⏲️Update Time : " +
    date;
  var options = {
    method: "post",
    payload: "message=" + msg,
    headers: { Authorization: "Bearer " + token },
  };
  UrlFetchApp.fetch("https://notify-api.line.me/api/notify", options);
}

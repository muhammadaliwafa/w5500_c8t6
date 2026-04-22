/*
 * html_pages.h
 *
 *  Created on: Nov 12, 2025
 *      Author: me
 */

#ifndef INC_HTML_PAGES_H_
#define INC_HTML_PAGES_H_

//const char upload_page[] =
//		"<!DOCTYPE html><html><body>"
//		"<h1>Upload File ke W25Q32</h1>"
//		"<form method='POST' action='/upload' enctype='multipart/form-data'>"
//		"<input type='hidden' id='filesize' name='filesize'>"
//		"<input type='text' id='addr' name='addr' placeholder='Alamat hex (mis. 0x000000)'><br><br>"
//		"<input type='file' id='file' name='file' onchange='updateFileSize()'><br><br>"
////
//
//		"<label>Ukuran file: <span id='sizeDisplay'>-</span></label><br><br>"
//
//
//		"<input type='submit' value='Upload'>"
//		"</form>"
//
//		"<script>"
//		"function updateFileSize() {"
//		"  var fileInput = document.getElementById('file');"
//		"  var sizeDisplay = document.getElementById('sizeDisplay');"
//		"  var hiddenInput = document.getElementById('filesize');"
//		"  if (fileInput.files.length > 0) {"
//		"    var size = fileInput.files[0].size;"
//		"    sizeDisplay.textContent = size + ' byte';"
//		"    hiddenInput.value = size;"
//		"  } else {"
//		"    sizeDisplay.textContent = '-';"
//		"    hiddenInput.value = '';"
//		"  }"
//		"}"
//		"</script>"
//
//		"</body></html>";

const char upload_page[] =
"<!DOCTYPE html><html><head><meta charset='UTF-8'>"
"<style>"
"body{margin:0;padding:0;background:#0a0f1c;font-family:Arial;color:#e4e8f0;"
"display:flex;justify-content:center;align-items:center;height:100vh;}"
".card{width:360px;padding:30px;border-radius:20px;"
"background:rgba(255,255,255,0.06);"
"backdrop-filter:blur(10px);"
"box-shadow:0 0 25px rgba(0,150,255,0.3),inset 0 0 15px rgba(255,255,255,0.05);"
"border:1px solid rgba(255,255,255,0.1);animation:fade 0.6s ease;}"
"@keyframes fade{from{opacity:0;transform:translateY(10px);}to{opacity:1;transform:none;}}"

"h1{text-align:center;font-size:22px;margin-bottom:20px;color:#63b3ff;"
"text-shadow:0 0 12px #63b3ff;}"

"input[type=text],input[type=file]{width:100%;padding:10px;margin-top:12px;"
"border-radius:12px;border:1px solid #2b3a4a;background:#141a27;color:#cfd6e3;"
"box-shadow:inset 0 0 8px rgba(0,0,0,0.4);transition:0.2s;}"
"input[type=text]:focus,input[type=file]:focus{border-color:#6dbaff;"
"box-shadow:0 0 8px #6dbaff,inset 0 0 8px rgba(0,0,0,0.4);} "

"label{display:block;margin-top:15px;color:#9ba3b5;font-size:14px;}"

"input[type=submit]{margin-top:25px;width:100%;padding:12px;border:none;border-radius:12px;"
"background:linear-gradient(45deg,#007acc,#5b2be0);color:#fff;font-weight:bold;"
"cursor:pointer;font-size:15px;letter-spacing:0.5px;"
"box-shadow:0 0 12px #007acc;transition:0.2s;}"
"input[type=submit]:hover{box-shadow:0 0 18px #5b2be0;opacity:0.92;}"

"#sizeDisplay{color:#8bc4ff;margin-top:6px;font-size:14px;}"
"</style></head><body>"

"<div class='card'>"
"<h1>Upload ke W25Q32</h1>"
"<form method='POST' action='/upload' enctype='multipart/form-data'>"

"<input type='hidden' id='filesize' name='filesize'>"

"<input type='text' id='addr' name='addr' placeholder='Alamat hex (0x000000)'>"

"<input type='file' id='file' name='file' onchange='u()'>"

"<label>Ukuran file:</label>"
"<span id='sizeDisplay'>-</span>"

"<input type='submit' value='Upload'>"
"</form>"
"</div>"

"<script>"
"function u(){var f=document.getElementById('file');"
"var d=document.getElementById('sizeDisplay');"
"var h=document.getElementById('filesize');"
"if(f.files.length>0){var s=f.files[0].size;d.textContent=s+' byte';h.value=s;}"
"else{d.textContent='-';h.value='';}}"
"</script>"

"</body></html>";


//const char upload_page[] =
//"<!DOCTYPE html>"
//"<html>"
//"<head>"
//    "<meta charset='UTF-8'>"
//    "<title>Upload ke STM32</title>"
//"</head>"
//"<body>"
//    "<input type='file' id='fileInput'>"
//    "<button onclick='uploadToSTM32()'>Upload ke STM32</button>"
//    "<div id='progress'></div>"
//    "<div id='status'></div>"
//
//    "<script>"
//    "async function uploadToSTM32() {"
//        "const fileInput = document.getElementById('fileInput');"
//        "const file = fileInput.files[0];"
//        "if (!file) return;"
//
//        "const CHUNK_SIZE = 1024;"
//        "const totalChunks = Math.ceil(file.size / CHUNK_SIZE);"
//
//        "for (let chunkIndex = 0; chunkIndex < totalChunks; chunkIndex++) {"
//            "const start = chunkIndex * CHUNK_SIZE;"
//            "const end = Math.min(start + CHUNK_SIZE, file.size);"
//            "const chunk = file.slice(start, end);"
//
//            "const formData = new FormData();"
//            "formData.append('chunk', chunk);"
//            "formData.append('chunk_index', chunkIndex);"
//            "formData.append('total_chunks', totalChunks);"
//            "formData.append('file_size', file.size);"
//            "formData.append('filename', file.name);"
//
//            "try {"
//                "const response = await fetch('/upload', {"
//                    "method: 'POST',"
//                    "body: formData"
//                "});"
//
//                "const result = await response.text();"
//				"console.log(result);"
//                "const progress = ((chunkIndex + 1) / totalChunks) * 100;"
//                "document.getElementById('progress').innerHTML = "
//                    "'Progress: ' + progress.toFixed(1) + '% (' + (chunkIndex + 1) + '/' + totalChunks + ')';"
//                "document.getElementById('status').innerHTML = "
//                    "'STM32 Response: ' + result;"
//
//                "if (result.includes('ERROR')) {"
//                    "alert('Upload failed: ' + result);"
//                    "break;"
//                "}"
//
//            "} catch (error) {"
//                "document.getElementById('status').innerHTML = "
//                    "'Error: ' + error.message;"
//                "break;"
//            "}"
//        "}"
//
////        "document.getElementById('status').innerHTML = 'Upload completed!';"
//    "}"
//    "</script>"
//"</body>"
//"</html>";
//
//
//const char post_page[] =
//"<!DOCTYPE html>"
//"<html><body>"
//"<h3>POST Form</h3>"
//"<form method='POST' action='/upload' enctype=\"text/plain\">"
//"Addr: <input name='addr' value='0x000000'><br>"
//"Value: <input name='value' value='123'><br>"
//"<input type='submit' value='Kirim'>"
//"</form>"
//"</body></html>";


#endif /* INC_HTML_PAGES_H_ */

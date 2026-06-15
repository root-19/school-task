const PDFDocument = require('./node_modules/pdfkit');
const fs = require('fs');

// ── A4 landscape for more horizontal room ──
const doc = new PDFDocument({ size: 'A4', layout: 'landscape', margin: 0, autoFirstPage: false });
doc.pipe(fs.createWriteStream(__dirname + '/system_flowchart.pdf'));

const PW = 841.89, PH = 595.28; // A4 landscape

// ─── COLOR PALETTE ───────────────────────────────────────────
const C = {
  start:  { f:'#1e8449', s:'#145a32', t:'#fff' },
  proc:   { f:'#1a6fa3', s:'#154e73', t:'#fff' },
  dec:    { f:'#6c3483', s:'#4a235a', t:'#fff' },
  err:    { f:'#cb4335', s:'#922b21', t:'#fff' },
  user:   { f:'#0e7b6c', s:'#0a5749', t:'#fff' },
  admin:  { f:'#b9770e', s:'#7d5109', t:'#fff' },
  logout: { f:'#566573', s:'#2c3e50', t:'#fff' },
  info:   { f:'#1f618d', s:'#154360', t:'#fff' },
  bg:     '#f5f6fa',
  arrow:  '#2c3e50',
  lbl:    '#1a5276',
};

// ─── DRAWING UTILS ────────────────────────────────────────────
function pill(d,cx,cy,w,h,col,...lines){
  d.save().roundedRect(cx-w/2,cy-h/2,w,h,h/2).fillAndStroke(col.f,col.s).restore();
  _txt(d,cx,cy,w,h,col.t,lines);
}
function box(d,cx,cy,w,h,col,...lines){
  d.save().roundedRect(cx-w/2,cy-h/2,w,h,6).fillAndStroke(col.f,col.s).restore();
  _txt(d,cx,cy,w,h,col.t,lines);
}
function diam(d,cx,cy,w,h,col,...lines){
  d.save()
   .moveTo(cx,cy-h/2).lineTo(cx+w/2,cy).lineTo(cx,cy+h/2).lineTo(cx-w/2,cy)
   .closePath().fillAndStroke(col.f,col.s).restore();
  _txt(d,cx,cy,w-20,h,col.t,lines);
}
function _txt(d,cx,cy,w,h,color,lines){
  if(!lines||!lines.length)return;
  const lh=13, tot=lines.length*lh;
  lines.forEach((ln,i)=>{
    d.fillColor(color)
     .font(i===0?'Helvetica-Bold':'Helvetica')
     .fontSize(i===0?9.5:8)
     .text(String(ln),cx-w/2+3,cy-tot/2+i*lh+2,{width:w-6,align:'center',lineBreak:false});
  });
}
function arrow(d,x1,y1,x2,y2,lbl,side){
  d.save().strokeColor(C.arrow).lineWidth(1.5).moveTo(x1,y1).lineTo(x2,y2).stroke();
  const a=Math.atan2(y2-y1,x2-x1),hs=7;
  d.fillColor(C.arrow)
   .moveTo(x2,y2)
   .lineTo(x2-hs*Math.cos(a-0.4),y2-hs*Math.sin(a-0.4))
   .lineTo(x2-hs*Math.cos(a+0.4),y2-hs*Math.sin(a+0.4))
   .closePath().fill().restore();
  if(lbl){
    const mx=(x1+x2)/2, my=(y1+y2)/2;
    const ox=side==='r'?5:side==='l'?-38:2, oy=side==='u'?-11:2;
    d.fillColor(C.lbl).fontSize(7.5).font('Helvetica-Bold').text(lbl,mx+ox,my+oy,{width:50});
  }
}
function seg(d,x1,y1,x2,y2){ d.save().strokeColor(C.arrow).lineWidth(1.5).moveTo(x1,y1).lineTo(x2,y2).stroke().restore(); }
function title(d,text){ d.fillColor('#1a2533').fontSize(13).font('Helvetica-Bold').text(text,30,18,{width:PW-60,align:'center'}); }
function pageHdr(d,n,t){ title(d,`School Management System — Page ${n}: ${t}`); }

// ─── PAGE BACKGROUND ─────────────────────────────────────────
function bg(d){ d.save().rect(0,0,PW,PH).fill(C.bg).restore(); }

// ══════════════════════════════════════════════════════════════
// PAGE 1 — MAIN FLOW  (Start → Main Menu → Login → Role)
// ══════════════════════════════════════════════════════════════
doc.addPage();
bg(doc);
pageHdr(doc,1,'Main Menu · Login · Register');

// vertical spine x = 420 (center of page)
const MX=420;
let y=50;

// START
pill(doc,MX,y,150,28,C.start,'START — Launch Application');
arrow(doc,MX,y+14,MX,y+30);

// Init
y+=46;
box(doc,MX,y,280,34,C.proc,'Initialize System','Connect SQLite DB · Create tables if missing');
arrow(doc,MX,y+17,MX,y+31);

// ── MAIN MENU ──────────────────────────────────────────────
y+=48;
const mmY=y;
box(doc,MX,y,200,70,C.info,'MAIN MENU','1. Login','2. Register','3. Exit');
arrow(doc,MX,y+35,MX,y+49);

y+=65;
const mmDecY=y+25;
diam(doc,MX,mmDecY,210,46,C.dec,'Main Menu Choice?');

// ── Branch: 3. Exit ──────────────────────────────────────────
const exitX=680;
seg(doc,MX+105,mmDecY,exitX,mmDecY);
arrow(doc,exitX,mmDecY,exitX,mmDecY+30,'3. Exit','r');
pill(doc,exitX,mmDecY+46,110,28,C.start,'EXIT','Goodbye!');

// ── Branch: 2. Register ──────────────────────────────────────
const regX=160;
seg(doc,MX-105,mmDecY,regX,mmDecY);
arrow(doc,regX,mmDecY,regX,mmDecY+30,'2. Register','l');

// Register flow (left column)
let ry=mmDecY+46;
box(doc,regX,ry,170,28,C.proc,'Enter: username','password, email');
arrow(doc,regX,ry+14,regX,ry+30); ry+=44;

diam(doc,regX,ry,170,48,C.dec,'Empty? Pass<6?','Name/Email exists?');
// YES → error
seg(doc,regX+85,ry,regX+130,ry);
arrow(doc,regX+130,ry,regX+130,ry+32,'YES','r');
box(doc,regX+130,ry+46,120,26,C.err,'Error msg','→ Main Menu');

// NO → save
arrow(doc,regX,ry+24,regX,ry+44,'NO','l'); ry+=60;
box(doc,regX,ry,170,28,C.user,'Save user to DB (role=USER)');
arrow(doc,regX,ry+14,regX,ry+30); ry+=44;
box(doc,regX,ry,170,24,C.logout,'Success → back to Main Menu');

// ── Branch: 1. Login ─────────────────────────────────────────
arrow(doc,MX,mmDecY+23,MX,mmDecY+43,'1. Login','r');
y=mmDecY+60;

box(doc,MX,y,260,28,C.proc,'Enter username & password');
arrow(doc,MX,y+14,MX,y+30); y+=44;

// Empty check
const errX=650;
diam(doc,MX,y,200,46,C.dec,'Fields empty?');
seg(doc,MX+100,y,errX,y);
arrow(doc,errX,y,errX,y+32,'YES','r');
box(doc,errX,y+46,120,28,C.err,'Error: required','attempts++');
arrow(doc,MX,y+23,MX,y+43,'NO','r'); y+=58;

// User found?
diam(doc,MX,y,200,46,C.dec,'User found in DB?');
seg(doc,MX+100,y,errX,y);
arrow(doc,errX,y,errX,y+32,'NO','r');
box(doc,errX,y+46,120,28,C.err,'Error: not found','attempts++');
arrow(doc,MX,y+23,MX,y+43,'YES','r'); y+=58;

// Password?
diam(doc,MX,y,200,46,C.dec,'Password correct?');
seg(doc,MX+100,y,errX,y);
arrow(doc,errX,y,errX,y+32,'NO','r');
box(doc,errX,y+46,120,28,C.err,'Error: wrong pass','attempts++');
arrow(doc,MX,y+23,MX,y+43,'YES','r'); y+=58;

// Attempts>=3?
diam(doc,MX,y,200,46,C.dec,'Attempts >= 3?');
seg(doc,MX+100,y,errX,y);
arrow(doc,errX,y,errX,y+32,'YES','r');
box(doc,errX,y+46,130,28,C.err,'Max attempts reached','→ Return to Main Menu');
// connect all error boxes vertically along errX
const errBoxes=[mmDecY+46+13, mmDecY+104+13, mmDecY+162+13, mmDecY+220+13];
// NO = retry
arrow(doc,MX,y+23,MX,y+43,'NO=retry','r'); y+=58;

// Role check
diam(doc,MX,y,200,44,C.dec,'User Role?');
const roleY=y;

// USER
const uMX=200;
seg(doc,MX-100,roleY,uMX,roleY);
arrow(doc,uMX,roleY,uMX,roleY+32,'USER','u');
box(doc,uMX,roleY+46,150,26,C.user,'→ USER MENU  (Page 2)');

// ADMIN
const adMX=640;
seg(doc,MX+100,roleY,adMX,roleY);
arrow(doc,adMX,roleY,adMX,roleY+32,'ADMIN','u');
box(doc,adMX,roleY+46,150,26,C.admin,'→ ADMIN MENU  (Page 3)');

// footer
doc.fillColor('#777').fontSize(7.5).font('Helvetica')
   .text('Page 1 of 3  ·  School Management System Flowchart',0,PH-18,{width:PW,align:'center'});

// ══════════════════════════════════════════════════════════════
// PAGE 2 — USER MENU + ENROLL STUDENT
// ══════════════════════════════════════════════════════════════
doc.addPage();
bg(doc);
pageHdr(doc,2,'User Menu · Enroll Student Detail');

// ── Left half: User Menu loop ────────────────────────────────
const UX=200;
y=52;
pill(doc,UX,y,180,26,C.user,'USER MENU  (entered after login)');
arrow(doc,UX,y+13,UX,y+27); y+=42;

box(doc,UX,y,190,80,C.info,
  'USER MENU',
  '1. View All Sessions',
  '2. View Available Slots',
  '3. Enroll Student',
  '4. View All Students',
  '5. Logout');
const umBottom=y+80;
arrow(doc,UX,umBottom,UX,umBottom+14); y=umBottom+30;

diam(doc,UX,y,195,46,C.dec,'Choice?');
const umDecY=y;

// Invalid → error loop back
seg(doc,UX+97,umDecY,UX+150,umDecY);
arrow(doc,UX+150,umDecY,UX+150,umDecY+42,'other','r');
box(doc,UX+150,umDecY+56,110,24,C.err,'Invalid choice!','Retry');
// loop back up to menu
seg(doc,UX+150+55,umDecY+56,UX+150+55,umBottom-10);
seg(doc,UX+150+55,umBottom-10,UX+95,umBottom-10);
arrow(doc,UX+95,umBottom-10,UX+95,umBottom-1);

// branches down
arrow(doc,UX,umDecY+23,UX,umDecY+43); y=umDecY+56;

diam(doc,UX,y,195,50,C.dec,'Which option?');
const optDecY=y;

// 5 option lines spread
const opts=[
  {x:UX-80,n:'1',lbl:'View Sessions'},
  {x:UX-40,n:'2',lbl:'View Slots'},
  {x:UX,   n:'3',lbl:'Enroll Student'},
  {x:UX+40,n:'4',lbl:'View Students'},
  {x:UX+80,n:'5',lbl:'Logout'},
];

// horizontal spread line
seg(doc,UX-80,optDecY+25,UX+80,optDecY+25);
opts.forEach(o=>{
  arrow(doc,o.x,optDecY+25,o.x,optDecY+43);
  doc.fillColor(C.lbl).fontSize(7).font('Helvetica-Bold').text(o.n,o.x-3,optDecY+28);
});

y=optDecY+58;
const boxH=36;
opts.forEach(o=>{
  if(o.n==='5'){
    box(doc,o.x,y,78,boxH,C.logout,'Logout',`→ Main Menu`);
  } else if(o.n==='3'){
    box(doc,o.x,y,80,boxH,C.user,'Enroll Student','(see right →)');
  } else {
    box(doc,o.x,y,80,boxH,C.proc,o.lbl,'Display table');
  }
});

// loop back for 1,2,3,4
const loopY=y+boxH+8;
[UX-80,UX-40,UX,UX+40].forEach(ox=>{
  seg(doc,ox,y+boxH,ox,loopY);
});
seg(doc,UX-80,loopY,UX+40,loopY);
const loopMid=(UX-80+UX+40)/2;
seg(doc,loopMid,loopY,loopMid,umBottom-18);
arrow(doc,loopMid,umBottom-18,UX,umBottom-18);
arrow(doc,UX,umBottom-18,UX,umBottom-1);

// ── Right half: Enroll Student detail ────────────────────────
const EX=600;
y=52;
doc.fillColor(C.user.f).fontSize(10).font('Helvetica-Bold')
   .text('ENROLL STUDENT — Detailed Flow',EX-130,y-2);
y+=18;

// step helper
const step=(lbl,col)=>{
  box(doc,EX,y,265,26,col||C.proc,...(Array.isArray(lbl)?lbl:[lbl]));
  arrow(doc,EX,y+13,EX,y+27); y+=40;
};
const check=(lines,col)=>{
  diam(doc,EX,y,265,48,col||C.dec,...(Array.isArray(lines)?lines:[lines]));
  const dy=y;
  // YES error on right
  seg(doc,EX+132,dy,EX+195,dy);
  arrow(doc,EX+195,dy,EX+195,dy+32,'YES','r');
  box(doc,EX+195,dy+46,110,24,C.err,'Error → return menu');
  // NO down
  arrow(doc,EX,dy+24,EX,dy+44,'NO','l');
  y=dy+58;
};

// 0. Session validity
step(['Check: is current user still in DB?']);
check(['User no longer valid?']);

// 1. Student Number
step(['Enter Student Number']);
check(['Empty? Already exists?']);

// 2. Name / Gender / Age
step(['Enter Name  ·  Gender  ·  Age']);
check(['Any field empty?','Age ≤ 0 or > 120?']);

// 3. Show sessions
box(doc,EX,y,265,34,C.info,
  'Display Available Sessions',
  '(section, modality, slots, name)');
arrow(doc,EX,y+17,EX,y+31); y+=48;

// 4. Pick session
step(['Select session by number']);
check(['Invalid selection?']);

// 5. Section & modality auto-set
box(doc,EX,y,265,26,C.proc,'section & modality taken from session');
arrow(doc,EX,y+13,EX,y+27); y+=40;

// 6. Save
box(doc,EX,y,265,34,C.user,'Save student to DB','Decrement available_slots by 1');
arrow(doc,EX,y+17,EX,y+31); y+=48;

// 7. Show summary
box(doc,EX,y,265,34,C.start,'Show enrollment summary','(number, name, section, modality, slots left)');
arrow(doc,EX,y+17,EX,y+31); y+=48;

box(doc,EX,y,220,24,C.user,'Return to USER MENU (loop)');

// footer
doc.fillColor('#777').fontSize(7.5).font('Helvetica')
   .text('Page 2 of 3  ·  School Management System Flowchart',0,PH-18,{width:PW,align:'center'});

// ══════════════════════════════════════════════════════════════
// PAGE 3 — ADMIN MENU + ALL SUB-FLOWS
// ══════════════════════════════════════════════════════════════
doc.addPage();
bg(doc);
pageHdr(doc,3,'Admin Menu · Session & Student Management');

// ── Top: Admin Menu loop ──────────────────────────────────────
const AMX=420;
y=48;
pill(doc,AMX,y,200,26,C.admin,'ADMIN MENU  (entered after login)');
arrow(doc,AMX,y+13,AMX,y+27); y+=42;

box(doc,AMX,y,220,90,C.info,
  'ADMIN MENU',
  '1. View All Sessions',
  '2. Add Session',
  '3. Update Session',
  '4. Delete Session',
  '5. View All Students',
  '6. Remove Student',
  '7. View All Users',
  '8. Logout');
const amBottom=y+90;
arrow(doc,AMX,amBottom,AMX,amBottom+14); y=amBottom+30;

diam(doc,AMX,y,200,46,C.dec,'Choice?');
const amDecY=y;
// invalid
seg(doc,AMX+100,amDecY,AMX+155,amDecY);
arrow(doc,AMX+155,amDecY,AMX+155,amDecY+38,'other','r');
box(doc,AMX+155,amDecY+52,110,24,C.err,'Invalid choice!','Retry');
seg(doc,AMX+155+55,amDecY+52,AMX+155+55,amBottom-10);
seg(doc,AMX+155+55,amBottom-10,AMX+100,amBottom-10);
arrow(doc,AMX+100,amBottom-10,AMX+100,amBottom);

arrow(doc,AMX,amDecY+23,AMX,amDecY+43); y=amDecY+56;

diam(doc,AMX,y,200,50,C.dec,'Which option?');
const aOptY=y;

// 8 options across the page
const aopts=[55,140,220,300,380,470,560,640];
const aoptL=['1','2','3','4','5','6','7','8'];
seg(doc,aopts[0],aOptY+25,aopts[7],aOptY+25);
aopts.forEach((ox,i)=>{ arrow(doc,ox,aOptY+25,ox,aOptY+43); doc.fillColor(C.lbl).fontSize(7).font('Helvetica-Bold').text(aoptL[i],ox-3,aOptY+28); });

y=aOptY+58;
const aboxH=38;
const aboxes=[
  {x:aopts[0],col:C.proc,lines:['View Sessions','Show all']},
  {x:aopts[1],col:C.admin,lines:['Add Session','(detail below)']},
  {x:aopts[2],col:C.admin,lines:['Update Session','(detail below)']},
  {x:aopts[3],col:C.err,lines:['Delete Session','(detail below)']},
  {x:aopts[4],col:C.proc,lines:['View Students','Show roster']},
  {x:aopts[5],col:C.err,lines:['Remove Student','(detail below)']},
  {x:aopts[6],col:C.proc,lines:['View Users','Show table']},
  {x:aopts[7],col:C.logout,lines:['Logout','→ Main Menu']},
];
aboxes.forEach(b=>{ box(doc,b.x,y,90,aboxH,b.col,...b.lines); });

// loop back for 1-7
const aLoopY=y+aboxH+8;
aopts.slice(0,7).forEach(ox=>{ seg(doc,ox,y+aboxH,ox,aLoopY); });
seg(doc,aopts[0],aLoopY,aopts[6],aLoopY);
const aMid=(aopts[0]+aopts[6])/2;
seg(doc,aMid,aLoopY,aMid,amBottom-14);
arrow(doc,aMid,amBottom-14,AMX,amBottom-14);
arrow(doc,AMX,amBottom-14,AMX,amBottom);

// ── DETAIL FLOWS bottom half ──────────────────────────────────
y=aLoopY+28;
doc.fillColor('#1a2533').fontSize(9).font('Helvetica-Bold')
   .text('─── Sub-flow Details ─────────────────────────────────────────────────────────────────────────────────',28,y-4);
y+=14;

// helper columns
const DC=[{x:90},{x:245},{x:400},{x:600},{x:755}];
const DW=130, DH_proc=24, DH_dec=42;

function dstep(col,lines,top){
  box(doc,col,top,DW,DH_proc,C.proc,...lines);
  arrow(doc,col,top+DH_proc/2,col,top+DH_proc/2+14);
  return top+DH_proc+16;
}
function dcheck(col,lines,top){
  diam(doc,col,top,DW,DH_dec,C.dec,...lines);
  // YES → err right
  seg(doc,col+DW/2,top,col+DW/2+22,top);
  arrow(doc,col+DW/2+22,top,col+DW/2+22,top+26,'YES','r');
  box(doc,col+DW/2+22,top+40,90,22,C.err,'Error → return');
  // NO down
  arrow(doc,col,top+DH_dec/2,col,top+DH_dec/2+14,'NO','l');
  return top+DH_dec+16;
}
function dend(col,lbl,top){
  box(doc,col,top,DW,24,C.start,lbl);
  arrow(doc,col,top+24,col,top+38);
  box(doc,col,top+40,DW,22,C.admin,'→ Admin Menu (loop)');
}

// ── Add Session ──
doc.fillColor(C.admin.f).fontSize(8.5).font('Helvetica-Bold').text('ADD SESSION',DC[0].x-DW/2+4,y);
let ty=y+14;
ty=dstep(DC[0].x,['Enter: name, section','slots, modality'],ty);
ty=dcheck(DC[0].x,['Empty? Slots≤0?','Name exists?'],ty);
box(doc,DC[0].x,ty,DW,24,C.admin,'Save session to DB');
arrow(doc,DC[0].x,ty+24,DC[0].x,ty+38); ty+=40;
box(doc,DC[0].x,ty,DW,22,C.start,'Success → Admin Menu');

// ── Update Session ──
doc.fillColor(C.admin.f).fontSize(8.5).font('Helvetica-Bold').text('UPDATE SESSION',DC[1].x-DW/2+4,y);
ty=y+14;
ty=dstep(DC[1].x,['Show sessions','Enter Session ID'],ty);
ty=dcheck(DC[1].x,['ID not found?'],ty);
ty=dstep(DC[1].x,['Enter new values','(blank = keep)'],ty);
ty=dcheck(DC[1].x,['New slots < enrolled?'],ty);
box(doc,DC[1].x,ty,DW,24,C.admin,'Update DB');
arrow(doc,DC[1].x,ty+24,DC[1].x,ty+38); ty+=40;
box(doc,DC[1].x,ty,DW,22,C.start,'Success → Admin Menu');

// ── Delete Session ──
doc.fillColor(C.err.f).fontSize(8.5).font('Helvetica-Bold').text('DELETE SESSION',DC[2].x-DW/2+4,y);
ty=y+14;
ty=dstep(DC[2].x,['Show sessions','Enter Session ID'],ty);
ty=dcheck(DC[2].x,['ID not found?'],ty);
box(doc,DC[2].x,ty,DW,28,C.err,'WARN: enrolled students','will also be deleted');
arrow(doc,DC[2].x,ty+28,DC[2].x,ty+42); ty+=44;
diam(doc,DC[2].x,ty,DW,38,C.dec,'Confirm? (y/n)');
seg(doc,DC[2].x+DW/2,ty,DC[2].x+DW/2+22,ty);
arrow(doc,DC[2].x+DW/2+22,ty,DC[2].x+DW/2+22,ty+26,'n','r');
box(doc,DC[2].x+DW/2+22,ty+40,90,22,C.logout,'Cancelled → return');
arrow(doc,DC[2].x,ty+19,DC[2].x,ty+39,'y','l'); ty+=55;
box(doc,DC[2].x,ty,DW,28,C.err,'Delete session','Cascade-remove students');
arrow(doc,DC[2].x,ty+28,DC[2].x,ty+42); ty+=44;
box(doc,DC[2].x,ty,DW,22,C.start,'Success → Admin Menu');

// ── Remove Student ──
doc.fillColor(C.err.f).fontSize(8.5).font('Helvetica-Bold').text('REMOVE STUDENT',DC[3].x-DW/2+4,y);
ty=y+14;
ty=dstep(DC[3].x,['Show all students','Enter Student ID'],ty);
ty=dcheck(DC[3].x,['ID not found?'],ty);
box(doc,DC[3].x,ty,DW,24,C.proc,'Show name/number','Confirm? (y/n)');
arrow(doc,DC[3].x,ty+24,DC[3].x,ty+38); ty+=40;
diam(doc,DC[3].x,ty,DW,38,C.dec,'Confirmed?');
seg(doc,DC[3].x+DW/2,ty,DC[3].x+DW/2+22,ty);
arrow(doc,DC[3].x+DW/2+22,ty,DC[3].x+DW/2+22,ty+26,'n','r');
box(doc,DC[3].x+DW/2+22,ty+40,90,22,C.logout,'Cancelled → return');
arrow(doc,DC[3].x,ty+19,DC[3].x,ty+39,'y','l'); ty+=55;
box(doc,DC[3].x,ty,DW,28,C.user,'Delete student from DB','Restore available_slot +1');
arrow(doc,DC[3].x,ty+28,DC[3].x,ty+42); ty+=44;
box(doc,DC[3].x,ty,DW,22,C.start,'Success → Admin Menu');

// ── Logout flow ──
doc.fillColor(C.logout.f).fontSize(8.5).font('Helvetica-Bold').text('LOGOUT',DC[4].x-DW/2+4,y);
ty=y+14;
box(doc,DC[4].x,ty,DW,24,C.logout,'User clicks Logout (option 5/8)');
arrow(doc,DC[4].x,ty+24,DC[4].x,ty+38); ty+=40;
box(doc,DC[4].x,ty,DW,24,C.proc,'Free currentUser memory','Set currentUser = null');
arrow(doc,DC[4].x,ty+24,DC[4].x,ty+38); ty+=40;
box(doc,DC[4].x,ty,DW,24,C.logout,'"Goodbye, [username]!"');
arrow(doc,DC[4].x,ty+24,DC[4].x,ty+38); ty+=40;
box(doc,DC[4].x,ty,DW,24,C.info,'Return to MAIN MENU loop');
arrow(doc,DC[4].x,ty+24,DC[4].x,ty+38); ty+=40;
pill(doc,DC[4].x,ty,DW,24,C.start,'Can login again or Exit');

doc.fillColor('#777').fontSize(7.5).font('Helvetica')
   .text('Page 3 of 3  ·  School Management System Flowchart',0,PH-18,{width:PW,align:'center'});

doc.end();
console.log('Flowchart saved → docs/system_flowchart.pdf');

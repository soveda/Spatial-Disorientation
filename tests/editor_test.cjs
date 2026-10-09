// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
// Test editor protocol and lifecycle with a simulated MIDI card, no hardware.
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const source=fs.readFileSync('web/index.html','utf8').match(/<script>([\s\S]*?)<\/script>/)[1];
const elements=new Map();
const downloads=[],urls=new Map();let urlId=0;
function element(){return {disabled:false,value:'',textContent:'',options:[],files:[],click(){if(this.download)downloads.push({name:this.download,blob:urls.get(this.href)});},setAttribute(){},append(o){this.options.push(o);if(!this.value)this.value=o.value;},replaceChildren(){this.options=[];this.value='';}};}
const defaults=[2048,1200,2048,2048,4095,4];
const firmwareDefaults=fs.readFileSync('src/config.h','utf8').match(/value\[kFields\]\s*=\s*\{([^}]+)\}/)[1].split(',').map(Number);
assert.deepEqual(defaults,firmwareDefaults);
const fields=defaults.map((v,i)=>({...element(),value:String(v),dataset:{id:String(i+1)},parentElement:{querySelector:()=>element()}}));
const document={getElementById:id=>{if(!elements.has(id))elements.set(id,element());return elements.get(id);},querySelectorAll:()=>fields,createElement:()=>element()};
let saved,active=[...defaults],sent=[];
const input={id:'in',name:'Spatial Disorientation',state:'connected',open:async()=>{},close:async()=>{}};
const output={id:'out',name:'Spatial Disorientation',state:'connected',open:async()=>{},close:async()=>{},send(msg){sent.push(msg);const cmd=msg[5],seq=msg[6];let body,reply;if(cmd===1){reply=65;body=active.flatMap((v,i)=>[i+1,v&127,v>>7]);}else{reply=66;body=[cmd,0];if(cmd===2)active=Array.from({length:6},(_,i)=>msg[8+i*3]|msg[9+i*3]<<7);if(cmd===3)saved=[...active];}queueMicrotask(()=>input.onmidimessage?.({data:[240,125,83,68,1,reply,seq,...body,247]}));}};
const midi={inputs:new Map([['in',input]]),outputs:new Map([['out',output]])};
const ctx=vm.createContext({document,navigator:{requestMIDIAccess:async()=>midi},Blob,URL:{createObjectURL(blob){const u='blob:'+ ++urlId;urls.set(u,blob);return u;},revokeObjectURL(u){urls.delete(u);}},setTimeout,clearTimeout,console});vm.runInContext(source,ctx);
(async()=>{
 const get=id=>document.getElementById(id);
 const load=async(text,size=Buffer.byteLength(text))=>{get('preset-file').files=[{size,text:async()=>text}];await get('preset-file').onchange();};
 // Export is usable before MIDI permission, and serializes the displayed values.
 get('preset-name').value='Near room';await get('export').onclick();
 assert.equal(downloads.length,1);assert.equal(downloads[0].name,'spatial-disorientation-near-room.json');
 const initialPreset=JSON.parse(await downloads[0].blob.text());
 assert.equal(initialPreset.format,'spatial-disorientation-preset');assert.equal(initialPreset.version,1);
 assert.equal(initialPreset.function,'twin-orbits');assert.deepEqual(Object.values(initialPreset.parameters),defaults);
 assert.equal(sent.length,0);
 assert(get('apply').disabled);assert(get('init').disabled);await get('access').onclick();assert(!get('connect').disabled);
 await get('connect').onclick();assert(!get('apply').disabled);assert(!get('save').disabled);
 fields[0].value='1024';fields[0].oninput();assert(get('save').disabled);
 await get('apply').onclick();assert.equal(active[0],1024);assert(!get('save').disabled);
 await get('save').onclick();assert.deepEqual(saved,active);
 const beforeSave=[...saved],saveCommands=sent.filter(m=>m[5]===3).length;
 fields[1].value='4095';fields[1].oninput();
 await get('init').onclick();assert.deepEqual(active,defaults);
 assert.deepEqual(fields.map(f=>Number(f.value)),defaults);
 assert.deepEqual(saved,beforeSave);assert.equal(sent.filter(m=>m[5]===3).length,saveCommands);
 assert(!get('save').disabled);
 await get('disconnect').onclick();assert(get('apply').disabled);assert(!get('connect').disabled);
 await get('connect').onclick();assert.equal(fields[0].value,2048);
 input.onmidimessage({data:[240,125,83,68,1,68,0,5,0,104,7,247]});
 assert(get('timing').textContent.includes('5 µs'));
 assert(get('timing').textContent.includes('1000 µs'));
 input.onmidimessage({data:[240,125,83,68,1,68,0,5,0,104,7,64,247]});
 assert(get('timing').textContent.includes('HRTF: 64 taps'));
 input.state='disconnected';await midi.onstatechange();assert(get('save').disabled);assert(get('init').disabled);
 // Stage a preset offline, connect without losing it, then explicit Apply/Save.
 const custom={...initialPreset,name:'Rear space',parameters:{'1':1024,'2':2000,'3':1500,'4':1800,'5':3000,'6':8}};
 const beforePreset=[...active],beforeMessages=sent.length;
 await load(JSON.stringify(custom));
 assert.deepEqual(fields.map(f=>Number(f.value)),Object.values(custom.parameters));
 assert.deepEqual(active,beforePreset);assert.equal(sent.length,beforeMessages);
 assert(get('save').disabled);assert.equal(get('preset-name').value,'Rear space');
 input.state='connected';await get('connect').onclick();
 assert.deepEqual(fields.map(f=>Number(f.value)),Object.values(custom.parameters));
 assert.deepEqual(active,beforePreset);assert(get('save').disabled);
 const stableFields=fields.map(f=>Number(f.value)),stableName=get('preset-name').value;
 const invalid=[
  '{',JSON.stringify(null),JSON.stringify({...custom,version:2}),
  JSON.stringify({...custom,function:'spatial-mixer'}),JSON.stringify({...custom,name:'bad\nname'}),
  JSON.stringify({...custom,parameters:{...custom.parameters,'7':10}}),
  JSON.stringify({...custom,parameters:{...custom.parameters,'2':4096}}),
  JSON.stringify({...custom,parameters:{...custom.parameters,'6':3}}),
  JSON.stringify({...custom,parameters:{...custom.parameters,'1':'1024'}}),
  JSON.stringify({...custom,parameters:{...custom.parameters,'1':1.5}}),
  JSON.stringify({...custom,parameters:{'1':0}})
 ];
 for(const text of invalid){const count=sent.length;await load(text);assert.deepEqual(fields.map(f=>Number(f.value)),stableFields);assert.equal(get('preset-name').value,stableName);assert.equal(sent.length,count);assert(get('save').disabled);}
 await load(JSON.stringify(custom),16385);assert(get('status').textContent.includes('too large'));assert.deepEqual(fields.map(f=>Number(f.value)),stableFields);
 assert.equal(get('preset-file').value,''); // Same file can be chosen again.
 await get('export').onclick();const roundtrip=JSON.parse(await downloads.at(-1).blob.text());assert.deepEqual(roundtrip,custom);
 const saveCount=sent.filter(m=>m[5]===3).length;
 await get('apply').onclick();assert.deepEqual(active,Object.values(custom.parameters));assert(!get('save').disabled);
 assert.equal(sent.filter(m=>m[5]===3).length,saveCount);
 await get('save').onclick();assert.deepEqual(saved,active);
 // Explicit Read still discards staged edits, while reconnect retains them.
 await load(JSON.stringify(initialPreset));await get('read').onclick();assert.deepEqual(fields.map(f=>Number(f.value)),saved);assert(!get('save').disabled);
 assert.throws(()=>vm.runInContext('decode([1,0,0,1,0,0,3,0,0,4,0,0,5,0,0,6,4,0])',ctx));
 assert.throws(()=>vm.runInContext('encode([4096,0,0,0,0,4])',ctx));
 for(const msg of sent)assert(msg.slice(1,-1).every(b=>b>=0&&b<=127));
 console.log('PASS: editor lifecycle, factory reset, offline preset roundtrip, validation/atomic rejection, staged reconnect, explicit Apply/Save and Read discard');
})().catch(e=>{console.error(e);process.exitCode=1;});

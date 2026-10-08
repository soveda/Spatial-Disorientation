// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
// Test editor protocol and lifecycle with a simulated MIDI card, no hardware.
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const source=fs.readFileSync('web/index.html','utf8').match(/<script>([\s\S]*?)<\/script>/)[1];
const elements=new Map();
function element(){return {disabled:false,value:'',textContent:'',options:[],setAttribute(){},append(o){this.options.push(o);if(!this.value)this.value=o.value;},replaceChildren(){this.options=[];this.value='';}};}
const defaults=[2048,1200,2048,2048,4095,4];
const firmwareDefaults=fs.readFileSync('src/config.h','utf8').match(/value\[kFields\]\s*=\s*\{([^}]+)\}/)[1].split(',').map(Number);
assert.deepEqual(defaults,firmwareDefaults);
const fields=defaults.map((v,i)=>({...element(),value:String(v),dataset:{id:String(i+1)},parentElement:{querySelector:()=>element()}}));
const document={getElementById:id=>{if(!elements.has(id))elements.set(id,element());return elements.get(id);},querySelectorAll:()=>fields,createElement:()=>element()};
let saved,active=[...defaults],sent=[];
const input={id:'in',name:'Spatial Disorientation',state:'connected',open:async()=>{},close:async()=>{}};
const output={id:'out',name:'Spatial Disorientation',state:'connected',open:async()=>{},close:async()=>{},send(msg){sent.push(msg);const cmd=msg[5],seq=msg[6];let body,reply;if(cmd===1){reply=65;body=active.flatMap((v,i)=>[i+1,v&127,v>>7]);}else{reply=66;body=[cmd,0];if(cmd===2)active=Array.from({length:6},(_,i)=>msg[8+i*3]|msg[9+i*3]<<7);if(cmd===3)saved=[...active];}queueMicrotask(()=>input.onmidimessage?.({data:[240,125,83,68,1,reply,seq,...body,247]}));}};
const midi={inputs:new Map([['in',input]]),outputs:new Map([['out',output]])};
const ctx=vm.createContext({document,navigator:{requestMIDIAccess:async()=>midi},setTimeout,clearTimeout,console});vm.runInContext(source,ctx);
(async()=>{
 const get=id=>document.getElementById(id);
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
 assert.throws(()=>vm.runInContext('decode([1,0,0,1,0,0,3,0,0,4,0,0,5,0,0,6,4,0])',ctx));
 assert.throws(()=>vm.runInContext('encode([4096,0,0,0,0,4])',ctx));
 for(const msg of sent)assert(msg.slice(1,-1).every(b=>b>=0&&b<=127));
 console.log('PASS: editor connect/read/apply/save/reconnect, dirty-state guard, disconnect, MIDI encoding, invalid data and factory reset without flash save');
})().catch(e=>{console.error(e);process.exitCode=1;});

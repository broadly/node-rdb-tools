// Copyright 2013 Danny Yates

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

var Parser = require('../rdb-tools').Parser,
    Writer = require('../rdb-tools').Writer,
    assert = require('chai').assert,
    fs = require('fs'),
    Writable = require('stream').Writable,
    Transform = require('stream').Transform,
    _ = require('underscore');

describe('Writer', function() {
    describe('should round-trip', function() {
        _.each(fs.readdirSync('test/dumps'), function(f) {
           it(f, roundTrip.bind(null, f));
        });
    })
});

function roundTrip(f, done) {
    var inputStream = fs.createReadStream('test/dumps/' + f),
        parser = new Parser(),
        inputCaptor = new Transform({objectMode: true}),
        writer = new Writer(),
        reparser = new Parser(),
        outputCaptor = new Writable({objectMode: true}),
        inputCaptives = [],
        outputCaptives = [];

    inputCaptor._transform = function(obj, encoding, cb) {
        inputCaptives.push(obj);
        this.push(obj);
        cb();
    }

    outputCaptor._write = function(obj, encoding, cb) {
        outputCaptives.push(obj);
        cb();
    }

    outputCaptor.on('finish', function() {
        // TODO: check the input and output captive match
        done('not yet testing');
    });

    inputStream.pipe(parser).pipe(inputCaptor).pipe(writer).pipe(reparser).pipe(outputCaptor);
}

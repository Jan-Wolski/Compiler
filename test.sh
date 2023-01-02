#!/bin/bash
for i in programs/*
do
	echo "Testing: $i"
	./project/kompilator "$i"
done


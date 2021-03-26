// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniFlag.h"



void AMiniFlag::Capture()	{
	AFlag::Capture();
	this->Destroy();
}

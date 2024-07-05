// Fill out your copyright notice in the Description page of Project Settings.


#include "JobIncome.h"

void FJobIncome::SetEverythingToZero()
{
	Foraging = 0;
	Woodcutting = 0;
	Hunting = 0;
	Converting = 0;
	Expanding = 0;
}

FJobIncome& FJobIncome::operator+=(const FJobIncome& Other)
{
	Foraging += Other.Foraging;
	Woodcutting += Other.Woodcutting;
	Hunting += Other.Hunting;
	Converting += Other.Converting;
	Expanding += Other.Expanding;
	return *this;
}
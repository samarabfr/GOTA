// Fill out your copyright notice in the Description page of Project Settings.


#include "GotaImage.h"

void UGotaImage::SetImage(UTexture2D* NewImage)
{
	if (NewImage)
	{
		Image->SetBrushFromTexture(NewImage);
	}
}

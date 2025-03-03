// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "GotaImage.generated.h"

/**
 * A Simple Widget to Show an Image
 */
UCLASS()
class GOTA_API UGotaImage : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

public:
	void SetImage(UTexture2D* NewImage);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UAbstractSkill;
class FSkillPropertiesInspector;
class USkillsContainerComponent;

/** \brief This class is friend of USkillsContainerComponent. Files who have visibility of this class are allowed to peek at USkillsContainerComponent's
 * properties. The reasons this class exists are:
 * 1. To give AI knowledge on all the possible skills at its disposal, so that it can take decisions on what to do.
 * 2. To have the UI w all the possible skills at its disposal, so that the player can take decisions on what to do.
 *
 * IMPORTANT: THIS CLASS SHALL BE HIDDEN TO EVERY FILE THAT IS NOT AI OR UI RELATED.
 */
class SKILLINSPECTION_API FSkillsContainerInspector {
public:
    FSkillsContainerInspector(TObjectPtr<USkillsContainerComponent> const& skillsContainer);

    TOptional<FSkillPropertiesInspector> GetSkillPropertiesByIndex(int32 const skillIndex) const;

    TOptional<int32> GetIndexOfSkill(TSubclassOf<UAbstractSkill> const& skillClass) const;

    void ForEachSkill(TFunction<void(FSkillPropertiesInspector const&, int32)> const skillPropertiesFunc) const;

private:
    TWeakObjectPtr<USkillsContainerComponent> _skillsContainer = nullptr;
};

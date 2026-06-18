/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2026 AUTHOR,AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fixedValueSectionFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::scalar Foam::fixedValueSectionFvPatchScalarField::t() const
{
    return db().time().timeOutputValue();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fixedValueSectionFvPatchScalarField::
fixedValueSectionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    x0_(0),
    x1_(0),
    BCValue_(0)
{
    refValue() = Zero;
    refGrad() = Zero;
    valueFraction() = Zero;
}


Foam::fixedValueSectionFvPatchScalarField::
fixedValueSectionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    x0_(dict.get<scalar>("x0")),
    x1_(dict.get<scalar>("x1")),
    BCValue_(dict.get<scalar>("BCValue"))
{
    refGrad() = Zero;
    valueFraction() = Zero;
    refValue() = Zero;
    evaluate();
}


Foam::fixedValueSectionFvPatchScalarField::
fixedValueSectionFvPatchScalarField
(
    const fixedValueSectionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    BCValue_(ptf.BCValue_)
{}


Foam::fixedValueSectionFvPatchScalarField::
fixedValueSectionFvPatchScalarField
(
    const fixedValueSectionFvPatchScalarField& ptf
)
:
    mixedFvPatchScalarField(ptf),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    BCValue_(ptf.BCValue_)
{}


Foam::fixedValueSectionFvPatchScalarField::
fixedValueSectionFvPatchScalarField
(
    const fixedValueSectionFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(ptf, iF),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    BCValue_(ptf.BCValue_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fixedValueSectionFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);
}


void Foam::fixedValueSectionFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);
}


void Foam::fixedValueSectionFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const vectorField& Cf = patch().Cf();

    forAll(Cf, facei)
    {
        const scalar x = Cf[facei].x();

        if (x >= x0_ && x <= x1_)
        {
            // Fixed value
            refValue()[facei] = BCValue_;
            refGrad()[facei] = 0;
            valueFraction()[facei] = 1; // 0 = fixed gradient, 1 = fixed value
        }
        else
        {
            // Zero gradient elsewhere
            refValue()[facei] = 0;
            refGrad()[facei] = 0;
            valueFraction()[facei] = 0;
        }
    }

    mixedFvPatchScalarField::updateCoeffs();
}


void Foam::fixedValueSectionFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);
    os.writeEntry("x0", x0_);
    os.writeEntry("x1", x1_);
    os.writeEntry("BCValue", BCValue_);
}


// * * * * * * * * * * * * * * Build Macro Function  * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        fixedValueSectionFvPatchScalarField
    );
}

// ************************************************************************* //

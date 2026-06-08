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

#include "partialTemperatureFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::partialTemperatureFvPatchScalarField::
partialTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    x0_(0),
    x1_(0),
    Tvalue_(0)
{
    refValue() = Zero;
    refGrad() = Zero;
    valueFraction() = Zero;
}


Foam::partialTemperatureFvPatchScalarField::
partialTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    x0_(dict.get<scalar>("x0")),
    x1_(dict.get<scalar>("x1")),
    Tvalue_(dict.get<scalar>("Tvalue"))
{
    refGrad() = Zero;
    valueFraction() = Zero;
    refValue() = Zero;
    evaluate();
}


Foam::partialTemperatureFvPatchScalarField::
partialTemperatureFvPatchScalarField
(
    const partialTemperatureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    Tvalue_(ptf.Tvalue_)
{}


Foam::partialTemperatureFvPatchScalarField::
partialTemperatureFvPatchScalarField
(
    const partialTemperatureFvPatchScalarField& ptf
)
:
    mixedFvPatchScalarField(ptf),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    Tvalue_(ptf.Tvalue_)
{}


Foam::partialTemperatureFvPatchScalarField::
partialTemperatureFvPatchScalarField
(
    const partialTemperatureFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(ptf, iF),
    x0_(ptf.x0_),
    x1_(ptf.x1_),
    Tvalue_(ptf.Tvalue_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::partialTemperatureFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);
}


void Foam::partialTemperatureFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);
}


void Foam::partialTemperatureFvPatchScalarField::updateCoeffs()
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
            // Fixed temperature strip
            refValue()[facei] = Tvalue_;
            refGrad()[facei] = 0; // if desired, could set to non-zero to specify gradient in the strip
            valueFraction()[facei] = 1; // and flip this to 0 to specify gradient instead of value
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


void Foam::partialTemperatureFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);
    os.writeEntry("x0", x0_);
    os.writeEntry("x1", x1_);
    os.writeEntry("Tvalue", Tvalue_);
    fvPatchScalarField::writeValueEntry(os);
}


// * * * * * * * * * * * * * * Build Macro Function  * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        partialTemperatureFvPatchScalarField
    );
}

// ************************************************************************* //

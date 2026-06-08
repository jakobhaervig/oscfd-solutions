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

Class
    Foam::partialTemperatureFvPatchScalarField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition applies a fixed temperature \c Tvalue between
    two y-coordinates \c y0 and \c y1 on the patch, and zeroGradient
    elsewhere.

Usage
    \table
        Property | Description                        | Req'd | Default
        y0       | lower y-coordinate of heated strip | yes   |
        y1       | upper y-coordinate of heated strip | yes   |
        Tvalue   | temperature applied in the strip   | yes   |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type    partialTemperature;
        y0      0.2;
        y1      0.8;
        Tvalue  400;
        value   uniform 300; // optional initial value
    }
    \endverbatim

SourceFiles
    partialTemperatureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef partialTemperatureFvPatchScalarField_H
#define partialTemperatureFvPatchScalarField_H

#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
     Class partialTemperatureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class partialTemperatureFvPatchScalarField
:
    public mixedFvPatchScalarField
{
    // Private Data

        //- Lower x-coordinate of the heated strip
        scalar x0_;

        //- Upper x-coordinate of the heated strip
        scalar x1_;

        //- Temperature applied between x0 and x1
        scalar Tvalue_;


    // Private Member Functions

public:

    //- Runtime type information
    TypeName("partialTemperature");


    // Constructors

        //- Construct from patch and internal field
        partialTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        partialTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        partialTemperatureFvPatchScalarField
        (
            const partialTemperatureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Copy construct
        partialTemperatureFvPatchScalarField
        (
            const partialTemperatureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new partialTemperatureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        partialTemperatureFvPatchScalarField
        (
            const partialTemperatureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new partialTemperatureFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member Functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //

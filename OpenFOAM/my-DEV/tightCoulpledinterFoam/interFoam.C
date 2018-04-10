/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
     \\/     M anipulation  |
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

Application
    interFoam

Description
    Solver for 2 incompressible, isothermal immiscible fluids using a VOF
    (volume of fluid) phase-fraction based interface capturing approach.

    The momentum and other fluid properties are of the "mixture" and a single
    momentum equation is solved.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

    For a two-fluid approach see twoPhaseEulerFoam.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"    //�����������ɢ����ļ�
#include "CMULES.H"  //����MULES������⣬OpenFOAM������һ�ְ�����ɢ��ʽ
#include "EulerDdtScheme.H"  //ŷ��ʱ����ɢ��ʽ
#include "localEulerDdtScheme.H"
#include "CrankNicolsonDdtScheme.H"
#include "subCycle.H"  //������ʱ�䲽
#include "immiscibleIncompressibleTwoPhaseMixture.H"  //����ѹ��˫�ഫ��ģ�ͣ�alpha���̣�
#include "turbulentTransportModel.H"  //����ģ��
#include "pimpleControl.H"
#include "fvIOoptionList.H"   //Դ��
#include "CorrectPhi.H"  //ͨ������
#include "fixedFluxPressureFvPatchScalarField.H"  //���ñ߽�ѹ������
#include "localEulerDdtScheme.H"
#include "fvcSmooth.H"  //��˳���

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"  //����caseĿ¼
    #include "createTime.H"  //����ʱ�����
    #include "createMesh.H"  //�����������

    pimpleControl pimple(mesh);  //����pimple�㷨

    #include "createTimeControls.H"  //����ʱ�����
    #include "createRDeltaT.H"  //����ʱ�䲽��LTS��
    #include "initContinuityErrs.H"  //��ʼ���������
    #include "createFields.H"  //����������
    #include "createMRF.H"  //���±߽��ٶ�
    #include "createFvOptions.H"  //����Դ��
    #include "correctPhi.H"  //������������  ��֤������

    if (!LTS)  //����ǷǾֲ�ʱ�䲽  ��LTS��deltaTΪ�Ǿ�һ�ĳ���,��ʼ����һ��
    {
        #include "readTimeControls.H"  //��ȡʱ�����
        #include "CourantNo.H"  //���������
        #include "setInitialDeltaT.H"  //��ʼʱ�䲽
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;  //�����ʾ��Ϣ   ��ʼʱ��ѭ��

    while (runTime.run())
    {
        #include "readTimeControls.H"  //��ȡʱ�����

        if (LTS)
        {
            #include "setRDeltaT.H"  //����LTS��ʱ�䲽
        }
        else  //����ÿһrunʱ��ļ����ʱ�䲽��������
        {
            #include "CourantNo.H"  //���������
            #include "alphaCourantNo.H"  //����alpha������
            #include "setDeltaT.H"  //����ʱ�䲽
        }

        runTime++;  //ʱ�����

        Info<< "Time = " << runTime.timeName() << nl << endl;  //�����ǰ����ʱ���

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())    //�����ٶ�ѹ��pimple��ѭ��
        {
            #include "alphaControls.H"  //��ȡalpha����  ��MULES�������ơ���ѭ������������ѹ�����ӵȣ�
            #include "alphaEqnSubCycle.H"  //alpha�������

            mixture.correct();  //1����������  2��ƽ���˶�ճ������

            #include "UEqn.H"  //����ٶȷ��̣�����������Ԥ�⣬��õ�һ��Ԥ���U

            // --- Pressure corrector loop
            while (pimple.correct())  //��ѭ��
            {
                #include "pEqn.H"  //���ѹ�����̣���������һ��ʱ�䲽���ڵ�P��p_rgh
            }

            if (pimple.turbCorr()) //������������Ŀ��ز�Ϊ��
            {
                turbulence->correct();  //�����Ķ����
            }
        }

        runTime.write();  //д������

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
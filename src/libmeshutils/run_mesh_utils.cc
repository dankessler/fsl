//#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include <fstream>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include "math.h"


#include "miscmaths/f2z.h"
#include "libprob.h"


#include "meshUtils.h"
#include "first_lib/first_mesh.h"
#include "first_lib/first_newmat_vec.h"

#include "fslvtkio/fslvtkio.h"
//#include "shapeModel/shapeModel.h"
#include "miscmaths/miscprob.h"
#include "utils/options.h"
#include "newimage/newimageall.h"
#include "meshclass/meshclass.h"
#include "warpfns/fnirt_file_reader.h"
#include "basisfield/basisfield.h"
#include "MVdisc/MVdisc.h"


using namespace std;
using namespace NEWIMAGE;
using namespace MISCMATHS;

using namespace Utilities;
using namespace mesh;
using namespace fslvtkio;
using namespace meshutils;
//using namespace shapemodel;
using namespace FIRST_LIB;
using namespace mvdisc;
string title="run_mesh_utils (Version 1.0)\nCopyright(c) 2008, University of Oxford (Brian Patenaude)";
string examples="run_mesh_utils [options] ";


Option<bool> verbose(string("-v,--verbose"), false, 
		     string("switch on diagnostic messages"), 
		     false, no_argument);
Option<bool> help(string("-h,--help"), false,
		  string("display this message"),
		  false, no_argument);
Option<bool> sampNN(string("--sampNN,--help"), false,
		    string("display this message"),
		    false, no_argument);
		  

Option<bool> toggle(string("--toggle,--toggle"), false,
		    string("toggle a feature"),
		    false, no_argument);
		  
		  
Option<int> myindex(string("-a,--myindex"), 0,
		    string("degrees of freedom"),
		    false, requires_argument);			  
Option<int> dof(string("-d,--dof"), 6,
		string("degrees of freedom"),
		false, requires_argument);
Option<int> dof2(string("-e,--dof2"), 6,
		 string("degrees of freedom2"),
		 false, requires_argument);	
Option<string> inname(string("-i,--in"), string(""),
		      string("filename of input image"),
		      false, requires_argument);
Option<string> flirtmatname(string("-f"), string(""),
			    string("filename of flirt matrix"),
			    false, requires_argument);
Option<string> inname2(string("-j"), string(""),
		       string("filename ofsecond input image"),
		       false, requires_argument);		  


Option<int> label(string("-l,--label"),1,
		  string("mesh label"),
		  false, requires_argument);

Option<string> inmeshname(string("-m"), string(""),
			  string("filename of base mesh"),
			  true, requires_argument);
Option<string> inmeshname2(string("-n"), string(""),
			   string("filename of base mesh2"),
			   false, requires_argument);

Option<string> outname(string("-o,--out"), string(""),
		       string("filename of output image"),
		       true, requires_argument);

Option<float> thresh(string("-t,--thresh"), 0,
		     string("threshold"),
		     false, requires_argument);	

Option<float> shiftx(string("-x,--xshift"), 0,
		     string("filename of input image"),
		     false, requires_argument);
Option<float> shifty(string("-y,--yshift"), 0,
		     string("filename of input image"),
		     false, requires_argument);
Option<float> shiftz(string("-z,--zshift"), 0,
		     string("filename of input image"),
		     false, requires_argument);			  

Option<float> w_im(string("-p,--w_im"), 0,
		   string("weighting image force"),
		   false, requires_argument);		

Option<float> w_tan(string("-r,--w_tan"), 0,
		    string("weighting image force"),
		    false, requires_argument);		
					 
Option<float> w_tri(string("-q,--w_tri"), 0,
		    string("weighting image force"),
		    false, requires_argument);		
					 
Option<float> w_norm(string("-s,--w_norm"), 0,
		     string("weighting image force"),
		     false, requires_argument);		


Option<bool> doMeshReg(string("--doMeshReg,--meshreg"), false,
		       string("display this message"),
		       false, no_argument);
Option<bool> doFillMesh(string("--doFillMesh,--fillmesh"), false,
			string("display this message"),
			false, no_argument);
Option<bool> doMeshToContours(string("--doMeshToContours,--meshToCont"), false,
			      string("convert mesh to a series of 2D contours"),
			      false, no_argument);
Option<bool> doCombineMeshes(string("--doCombineMeshes,--combMeshes"), false,
			     string("combine mesh results including vectors and scalars"),
			     false, no_argument);
Option<bool> doUnCentreMesh(string("--doUnCentreMesh,doUnCentreMesh"), false,
			    string("remove centre form ASCII vtk mesh (remove old IBIM format)"),
			    false, no_argument);
Option<bool>  doLabelAndCombineSB(string("--doLabelAndCombineSB, doLabelAndCombineSB"), false,
				  string("label shared vertices and combine meshes"),
				  false, no_argument);			  
							
Option<bool>  doSubtractMeshes(string("--doSubtractMeshes, doSubtractMeshes"), false,
			       string("doSubtractMeshes"),
			       false, no_argument);							  
								
Option<bool>  doAppendSBmask(string("--doAppendSBmask, doAppendSBmask"), false,
			     string("doAppendSBmask"),
			     false, no_argument);							    
			
Option<bool>  doAlterVertsBySBmask(string("--doAlterVertsBySBmask, doAlterVertsBySBmask"), false,
				   string("doAlterVertsBySBmask"),
				   false, no_argument);							 	  	
	
Option<bool>  doAppendIndexedSBmask(string("--doAppendIndexedSBmask, doAppendIndexedSBmask"), false,
				    string("doAppendIndexedSBmask"),
				    false, no_argument);	
		  
Option<bool>  useSc2(string("--useSc2, useSc2"), false,
		     string("useSc2"),
		     false, no_argument);	
		  
Option<bool>  inverse(string("--inverse, inverse"), false,
		      string("inverse"),
		      false, no_argument);	 
		  
		  		  
Option<bool>  doWarpMesh(string("--doWarpMesh,  doWarpMesh"), false,
			 string(" doWarpMesh"),
			 false, no_argument);	
		  
Option<bool>  doApplyFlirtThenSBmask(string("--doApplyFlirtThenSBmask, doApplyFlirtThenSBmask"), false,
				     string("doApplyFlirtThenSBmask"),
				     false, no_argument);		
Option<bool>  doSurfDistMap(string("--doSurfDistMap,  doSurfDistMap"), false,
			    string("doSurfDistMap"),
			    false, no_argument);	
		  
Option<bool>  doSurfMeanAndStDev(string("--doSurfMeanAndStDev,  doSurfMeanAndStDev"), false,
				 string("doSurfMeanAndStDev"),
				 false, no_argument);	
		  
Option<bool>  doLQSurfReg(string("--doLQSurfReg,  doLQSurfReg"), false,
			  string("doLQSurfReg"),
			  false, no_argument);												  
				
Option<bool>  doCartToSphere(string("--doCartToSphere,  doCartToSphere"), false,
			     string("doCartToSphere"),
			     false, no_argument);
				
Option<bool>  doSphereToCart(string("--doSphereToCart,  doSphereToCart"), false,
			     string("doSphereToCart"),
			     false, no_argument);		
		  	
Option<bool> doFindMidMidPoint(string("--doFindMidMidPoint, doFindMidMidPoint"), false,
			       string("doFindMidMidPoint"),
			       false, no_argument);			
				
Option<bool> doWarpGrid(string("--doWarpGrid, doWarpGrid"), false,
			string("doWarpGrid"),
			false, no_argument);		
Option<bool> doSampleGrid(string("--doSampleGrid, doSampleGrid"), false,
			  string("doSampleGrid"),
			  false, no_argument);
Option<bool> doSampleMesh(string("--doSampleMesh, doSampleMesh"), false,
			  string("doSampleMesh"),
			  false, no_argument);

Option<bool> doRandMesh(string("--doRandMesh, doRandMesh"), false,
			string("doRandMesh"),
			false, no_argument);								
					
Option<bool> doMeshToBvars(string("--doMeshToBvars, doMeshToBvars"), false,
			   string("doMeshToBvars"),
			   false, no_argument);	
										
Option<bool> doAddModesUsingScalars(string("--doAddModesUsingScalars, doAddModesUsingScalars"), false,
				    string("doAddModesUsingScalars"),
				    false, no_argument);	
				
Option<bool> doRandMatrices(string("--doRandMatrices, doRandMatrices"), false,
			    string("doRandMatrices"),
			    false, no_argument);	
				
Option<bool> doWriteConditionalIntensity(string("--doWriteConditionalIntensity, doWriteConditionalIntensity"), false,
					 string("doWriteConditionalIntensity"),
					 false, no_argument);	
			
Option<bool> doUgridToImage(string("--doUgridToImage, doUgridToImage"), false,
			    string("doUgridToImage"),
			    false, no_argument);	
			
Option<bool> doReplaceVertsWithCoef(string("--doReplaceVertsWithCoef, doReplaceVertsWithCoef"), false,
				    string("doReplaceVertsWithCoef"),
				    false, no_argument);

Option<bool> doCoefModelToImage(string("--doCoefModelToImage, doCoefModelToImage"), false,
				string("doCoefModelToImage"),
				false, no_argument);		
				
Option<bool> doFieldModelToImage(string("--doFieldModelToImage,doFieldModelToImage"), false,
				 string("doFieldModelToImage"),
				 false, no_argument);

Option<bool> doSubSampleGrid(string("--doSubSampleGrid,doSubSampleGrid"), false,
			     string("doSubSampleGrid"),
			     false, no_argument);

Option<bool> doScalarsToVolume(string("--doScalarsToVolume"), false,
			       string("doScalarsToVolume"),
			       false, no_argument);

Option<bool> doVectorsToVolume(string("--doVectorsToVolume"), false,
			       string("doVectorsToVolume"),
			       false, no_argument);

Option<bool> doPointsToVolume(string("--doPointsToVolume"), false,
			       string("doPointsToVolume"),
			       false, no_argument);

Option<bool> doVolumeToScalars(string("--doVolumeToScalars"), false,
			       string("doVolumeToScalars"),
			       false, no_argument);

Option<bool> doVolumeToVectors(string("--doVolumeToVectors"), false,
			       string("doVolumeToVectors"),
			       false, no_argument);

Option<bool> doFtoP(string("--doFtoP"), false,
		    string("doFtoP"),
		    false, no_argument);

Option<bool> doGetMeans(string("--doGetMeans"), false,
			string("doGetMeans"),
			false, no_argument);		
				
Option<bool> doFlipMesh(string("--doFlipMesh"), false,
			string("doFlipMesh"),
			false, no_argument);		
				
Option<bool> doAppendConstScalar(string("--doAppendConstScalar"), false,
				 string("doAppendConstScalar"),
				 false, no_argument);		
				
Option<bool> doShiftGrid(string("--doShiftGrid"), false,
			 string("doShiftGrid"),
			 false, no_argument);

Option<bool> doConcatIntensityGrid(string("--doConcatIntensityGrid"), false,
				   string("doConcatIntensityGrid"),
				   false, no_argument);

Option<bool> doDeMeanIntensities(string("--doDeMeanIntensities"), false,
				 string("doDeMeanIntensities"),
				 false, no_argument);
				
Option<bool> doConvert_ASCII_To_Binary(string("--doConvert_ASCII_To_Binary"), false,
				       string("doConvert_ASCII_To_Binary"),
				       false, no_argument);			
				
Option<bool> doConvert_Binary_To_ASCII(string("--doConvert_Binary_To_ASCII"), false,
				       string("doConvert_Binary_To_ASCII"),
				       false, no_argument);	
																		  
																	  		
Option<bool> doUnCentreModel(string("--doUnCentreModel"), false,
			     string(" doUnCentreModel"),
			     false, no_argument);	
		
Option<bool> doSampleAndNormalizeIntensities(string("--doSampleAndNormalizeIntensities"), false,
					     string(" doSampleAndNormalizeIntensities"),
					     false, no_argument);
			
Option<bool> doDeformSurface(string("--doDeformSurface"), false,
			     string(" doDeformSurface"),
			     false, no_argument);

Option<bool> doMeshReg_LeastSq(string("--doMeshReg_LeastSq"), false,
			       string(" doMeshReg_LeastSq"),
			       false, no_argument);

Option<bool> doGetMeshFromModel(string("--doGetMeshFromModel"), false,
				string("doGetMeshFromModel"),
				false, no_argument);
							
Option<bool> doVertexLDA_LOO(string("--doVertexLDA_LOO"), false,
			     string("doVertexLDA_LOO"),
			     false, no_argument);

Option<bool> doVertexLDA_save(string("--doVertexLDA_save"), false,
			      string("doVertexLDA_save"),
			      false, no_argument);	
				
Option<bool> doVertexLDA_loadAndApply(string("--doVertexLDA_loadAndApply"), false,
				      string("doVertexLDA_loadAndApply"),
				      false, no_argument);	
				
Option<bool> doGetMaxScalar(string("--doGetMaxScalar"), false,
			    string("doGetMaxScalar"),
			    false, no_argument);	
				
Option<bool> doGetMeanScalar(string("--doGetMeanScalar"), false,
			     string("doGetMeanScalar"),
			     false, no_argument);	
				
Option<bool> doAddScalars(string("--doAddScalars"), false,
			  string("doAddScalars"),
			  false, no_argument);	
				
Option<bool> doDivideScalarsByScalar(string("--doDivideScalarsByScalar"), false,
				     string("doDivideScalarsByScalar"),
				     false, no_argument);	
					 
				
Option<bool> doDisplayNumericFieldNames(string("--doDisplayNumericFieldNames"), false,
				     string("doDisplayNumericFieldNames"),
				     false, no_argument);					 
		Option<bool> doDisplayNumericField(string("--doDisplayNumericField"), false,
				     string("doDisplayNumericField"),
				     false, no_argument);					 
					 			 
int nonoptarg;

////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////

int main(int argc,char *argv[])
{
	
  Tracer tr("main");
  OptionParser options(title, examples);
	
  try {
    // must include all wanted options here (the order determines how
    //  the help message is printed)
    options.add(doUnCentreModel);
    options.add(inname);
    options.add(inname2);

    options.add(w_im);

    options.add(w_tan);
    options.add(w_norm);
    options.add(w_tri);

    options.add(inmeshname);
    options.add(inmeshname2);
    options.add(useSc2);
    options.add(flirtmatname);
    options.add(doMeshReg);
    options.add(doFillMesh);
    options.add(dof);
    options.add(dof2);
    options.add(toggle);
    options.add(label);
    options.add(doMeshToContours);
    options.add(outname);
    options.add(sampNN);
    options.add(doCombineMeshes);
    options.add(doUnCentreMesh);
    options.add(doLabelAndCombineSB);
    options.add(verbose);
    options.add(help);
    options.add(doSubtractMeshes);
    options.add(doAppendSBmask);
    options.add(doAppendIndexedSBmask);
    options.add(doApplyFlirtThenSBmask);
    options.add(inverse);
    options.add(doWarpMesh);
    options.add(shiftx);
    options.add(shifty);
    options.add(shiftz);
    options.add(doSurfDistMap);
    options.add(doSurfMeanAndStDev);
    options.add(doLQSurfReg);
    options.add(doAlterVertsBySBmask);
    options.add(doSphereToCart);
    options.add(doCartToSphere);
    options.add(doFindMidMidPoint);
    options.add(doWarpGrid);
    options.add(doSampleGrid);
    options.add(doSampleMesh);
    options.add(doRandMesh);
    options.add(doMeshToBvars);
    options.add(doAddModesUsingScalars);
    options.add(doRandMatrices);
    options.add(thresh);
    options.add(doWriteConditionalIntensity);
    options.add(doUgridToImage);
    options.add(doReplaceVertsWithCoef);
    options.add(doCoefModelToImage);
    options.add(myindex);
    options.add(doFieldModelToImage);
    options.add(doSubSampleGrid);
    options.add(doScalarsToVolume);
    options.add(doVectorsToVolume);
    options.add(doPointsToVolume);
    options.add(doVolumeToScalars);
    options.add(doVolumeToVectors);
    options.add(doFtoP);
    options.add(doGetMeans);
    options.add(doFlipMesh);
    options.add(doAppendConstScalar);
    options.add(doShiftGrid);
    options.add(doConcatIntensityGrid);
    options.add(doDeMeanIntensities);
    options.add(doConvert_ASCII_To_Binary);
    options.add(doConvert_Binary_To_ASCII);
    options.add(doSampleAndNormalizeIntensities);
    options.add(doDeformSurface);
    options.add(doMeshReg_LeastSq);
    options.add(doGetMeshFromModel);
    options.add(doVertexLDA_LOO);
    options.add(doVertexLDA_save);
    options.add(doVertexLDA_loadAndApply);
    options.add(doGetMeanScalar);
    options.add(doGetMaxScalar);
    options.add(doAddScalars);
    options.add(doDivideScalarsByScalar);
	options.add(doDisplayNumericFieldNames);
	options.add(doDisplayNumericField);
    nonoptarg = options.parse_command_line(argc, argv);
		
		// line below stops the program if the help was requested or 
		//  a compulsory option was not set
		if ( (help.value()) || (!options.check_compulsory_arguments(true)) )
		{
			options.usage();
			exit(EXIT_FAILURE);
		}
		// Call the local functions
		try{
		if (doConvert_ASCII_To_Binary.value())
		{
		//	meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
						fslvtkIO* m = new fslvtkIO();//inmeshname.value(),static_cast<fslvtkIO::DataType>(0));
		
						m->setSwitchRowsCols(toggle.value());
							m->readPolyData(inmeshname.value());

	cout<<"toggle "<<toggle.value()<<endl;
			m->setBinaryWrite(true);
			m->save(outname.value());

		
		}else if (doConvert_Binary_To_ASCII.value())
		{
		//	meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
						fslvtkIO* m = new fslvtkIO();//inmeshname.value(),static_cast<fslvtkIO::DataType>(0));
				m->setSwitchRowsCols(toggle.value());
							m->readPolyData(inmeshname.value());

	cout<<"toggle "<<toggle.value()<<endl;
			m->setBinaryWrite(false);
			m->save(outname.value());

		
		}else if (doUnCentreModel.value()){
			meshUtils* m = new meshUtils();
			m->setSwitchRowsCols(toggle.value());
			
			volume<float> im;
			read_volume(im,inname.value());//load label image
			float tx = (im.xsize()-1)/2.0*abs(im.xdim());
			float ty = (im.ysize()-1)/2.0*abs(im.ydim());
			float tz = (im.zsize()-1)/2.0*abs(im.zdim());
			cout<<"shift "<<tx<<" "<<ty<<" "<<tz<<endl;
			m->readPolyData(inmeshname.value());
						m->shiftPoints(tx,ty,tz );

			m->setBinaryWrite(true);
			m->save(outname.value());

		
		
		}else if (doFillMesh.value())
		{
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			volume<float> im;
			read_volume(im,inname.value());
			int bounds[6]={0,0,0,0,0,0};
			first_mesh::getBounds(m->getPointsAsVector<float>(),bounds,im.xdim(),im.ydim(),im.zdim());
			volume<short> mask=first_mesh::make_mask_from_mesh(im, m->getPointsAsVector<float>(), first_newmat_vector::matrixToVector<unsigned int>(m->getPolygons().t()), 1, bounds);
			save_volume(mask,outname.value());
			delete m;
		}else if (doSurfDistMap.value())
		{
			volume<short> imlb;
			read_volume(imlb,inname.value());//load label image
				meshUtils* m = new meshUtils;
		cout<<"load mesh"<<endl;
						m->loadMesh(inmeshname.value());
						cout<<"done load mesh"<<endl;

				vector<float> vdist;
				cout<<"surfdist"<<endl;
				m->SurfDistToLabels<float,short>(vdist,imlb,static_cast<short>(label.value()));
				m->setScalars(vdist);
				m->save(outname.value());
				delete m;
				
		}else if(doSurfMeanAndStDev.value())
		{
			meshUtils* m = new meshUtils;
			
			Matrix MeanPoints;
			Matrix MeanScalars;
			Matrix StDevScalars;
			m->SurfScalarsMeanAndStdev(meshUtils::fileToVector(inmeshname.value()),MeanPoints, MeanScalars,StDevScalars);
			m->setScalars(MeanScalars);
			m->addFieldData(StDevScalars,"std_dev","float");

			m->save(outname.value());
			delete m;
			
		}else if(doMeshReg.value())
		{
			meshUtils* m = new meshUtils;
			m->loadMesh(inmeshname.value());
			Matrix fmat=meshUtils::readFlirtMat(flirtmatname.value());
			if (inverse.value())
				fmat=fmat.i();
			m->meshReg(fmat);
			m->save(outname.value());
			delete m;
			
		}else if (doShiftGrid.value())
		{
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
			m->shiftPoints(shiftx.value(),shifty.value(),shiftz.value());
			m->save(outname.value());
			
			
		}else if (doLQSurfReg.value())
		{
			meshUtils* m = new meshUtils;
			m->loadMesh(inmeshname.value());
			
			meshUtils* mRef = new meshUtils;
			mRef->loadMesh(inmeshname2.value());
			
			Matrix fmat;
			m->LQSurfaceReg(mRef->getPointsAsMatrix(),fmat, dof.value());
			
			cout<<"final "<<fmat<<endl;
			
			meshUtils::writeFlirtMatrix(fmat, outname.value()+".mat");
			m->meshReg(fmat);
			m->save(outname.value()+".vtk");
			delete m;
			delete mRef;
		}else if (doCombineMeshes.value())
		{
			meshUtils m;
						
			m.combineMeshesWithVectorsAndScalars(meshUtils::fileToVector(inmeshname.value()));
			m.save(outname.value());
			//delete m;
		
		}else if (doMeshToContours.value())
		{
					 meshUtils* m ;
					// m= new meshUtils;
			cout<<"domeshtocontrous"<<endl;
			try
			{
				m=new meshUtils(inmeshname.value(),static_cast<fslvtkIO::DataType>(0));
				//cout<<"load Mesh"<<endl;
			//	m.loadMesh(inmeshname.value());
			}catch (exception& e)
			{
				e.what();
				return 1;
			}
			volume<char> im;
			read_volume(im,inname.value());
			
			vector<float> verts;
			verts=m->meshToContours(im,meshUtils::readFlirtMat(flirtmatname.value()));
			cout<<"foudn verts"<<endl;
			ofstream fout;
			fout.open(outname.value().c_str());
			short count=0;
			for (vector<float>::iterator i=verts.begin();i!=verts.end();i++){
				fout<<*i<<" ";
				if (count==2) { fout<<endl; count=0; }
				else count++;
			}
			fout.close();
			cout<<"done meshtoContours"<<endl;
			//delete m;
		}else if (doAppendSBmask.value())
		{
			meshUtils* m = new meshUtils;
			m->loadMesh(inmeshname.value());

			meshUtils* mRef = new meshUtils;
			mRef->loadMesh(inmeshname2.value());
			
			//replace the avgvertcies with the defored
			meshUtils* mnew = new meshUtils;
			mnew->loadMesh(inname.value());
			
			Matrix Sc=m->appendSharedBoundaryMask(mRef->getPointsAsMatrix());
			m->setPoints(mnew->getPointsAsMatrix());
			m->setScalars(Sc);
			m->save(outname.value());

			
			delete m;
			delete mRef;
			delete mnew;
		
		}else if (doAlterVertsBySBmask.value())
		{
			meshUtils* m = new meshUtils;
			m->loadMesh(inmeshname.value());

			meshUtils* mRef = new meshUtils;
			mRef->loadMesh(inmeshname2.value());
			
			m->sampleSharedBoundaryByMask(mRef->getPointsAsMatrix());
			m->save(outname.value());			
			delete m;

		}else if (doFindMidMidPoint.value())
		{
			meshUtils* m = new meshUtils;
			m->loadMesh(inmeshname.value());
	
			volume<char> im;
			read_volume(im,inname.value());
	
			float cx=0, cy=0, cz=0;
			m->findMidPointOfMidSlice(im,meshUtils::readFlirtMat(flirtmatname.value()),cx,cy,cz);
			cout<<"foudn verts"<<endl;
			ofstream fout;
			fout.open(outname.value().c_str());
			fout<<cx<<" "<<cy<<" "<<cz<<endl;
			fout.close();
			cout<<cx<<" "<<cy<<" "<<cz<<endl;
			delete m;

		}else if (doWarpGrid.value())
		{
			try
			{
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
			 
			 
			volume4D<float> warpField;
			read_volume4D(warpField,inname.value());
			m->warpGridWithDefField(warpField, shiftx.value(),shifty.value(),shiftz.value());
			m->save(outname.value());
			}catch(exception& e){
			
			cout<<e.what()<<endl;
			return 1;
			}
		
			
		}else if (doWarpMesh.value())
		{
			try
			{
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			 
			 
			volume4D<float> warpField;
			read_volume4D(warpField,inname.value());
			m->warpGridWithDefField(warpField, shiftx.value(),shifty.value(),shiftz.value());
			m->save(outname.value());
			}catch(exception& e){
			
			cout<<e.what()<<endl;
			return 1;
			}
		
			
		}else if (doSampleGrid.value())
		{
			cout<<"sample grid"<<endl;
			
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
			
		
		//	Matrix fmat=meshUtils::readFlirtMat(flirtmatname.value());
		//	m->meshReg(fmat.i());
			volume<float> image;
			vector<float> vsamples;
			read_volume(image,inname.value());
			m->sampleImageAtPoints<float>(image, vsamples);
			
			delete m;
			
			meshUtils* mout = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
			mout->setScalars<float>(vsamples);
			mout->save(outname.value());
			
			delete mout;
		}else if (doSampleMesh.value())
		{
			cout<<"sample grid"<<endl;
			
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix pts=m->getPointsAsMatrix();
		//	Matrix fmat=meshUtils::readFlirtMat(flirtmatname.value());
		//	m->meshReg(fmat.i());
			
			volume<float> image;
			vector<float> vsamples;
			read_volume(image,inname.value());
			m->sampleImageAtPoints<float>(image, vsamples);
			
			delete m;
			
		//	meshUtils* mout = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			m->setScalars<float>(vsamples);
			m->save(outname.value());
			
			delete m;
		}else if (doSampleAndNormalizeIntensities.value()){
		
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			// MJ: WHAT IS GOING ON HERE?? APPARENTLY NOTHING, BUT WHY?
		}else if (doRandMesh.value())
		{
			cout<<"sample grid"<<endl;
			
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix sc=m->getScalars();
			vector<bool> vsc;
			for (int i=0;i<sc.Nrows();i++)
			{
				if (sc.element(i,0)>thresh.value()){
				vsc.push_back(true);
				//						vsc.push_back(false);
			}else
					vsc.push_back(false);
			}
			
			Mesh m1;
			m1.load(inmeshname.value());
			Matrix points=m->getPointsAsMatrix();
			int count=0;
			for (vector<Mpoint*>::iterator i = m1._points.begin(); i!=m1._points.end(); i++ , count++)
			{
			(*i)->_update_coord = Pt(points.element(count,0), points.element(count,1),points.element(count,2)); 
				}	
				m1.update();
			meshUtils::generateRandomMeshUsingScalar(m1,outname.value(), vsc,40);
			delete m;
			
			
		}else if (doRandMatrices.value())
		{
			meshUtils::generateRandom6DOFMatrices( outname.value(), 40);

		}else if (doUgridToImage.value())
		{
				cout<<"convert grid to image"<<endl;
			
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
			
			volume<float> image;
			read_volume(image,inname.value());
			m->ugridToImage<float>(image);
		
			save_volume(image,outname.value());	
			delete m;
		
		}else if (doReplaceVertsWithCoef.value()){//used to play around 
				
				cout<<"replace verts"<<endl;
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));

				// MJ: WHAT IS GOING ON HERE?? APPARENTLY NOTHING, BUT WHY?
				
				
		//getcoefrep disappear		
		//		FnirtFileReader fr(inname.value());
				
		//		cout<<"number of ceofficient fields "<<fr.GetCoefRep().size()<<" "<<fr.GetCoefRep().at(0)->GetCoef()->Nrows()<<" "<<fr.GetCoefRep().at(1)->CoefSz()<<endl;
		//		Matrix Coefs=*(fr.GetCoefRep().at(0)->GetCoef()) & *(fr.GetCoefRep().at(1)->GetCoef()) & *(fr.GetCoefRep().at(2)->GetCoef());
		//		cout<<"Coefs "<<Coefs.Nrows()<<" "<<Coefs.Ncols()<<endl;
		//		m->setPoints(*(fr.GetCoefRep().at(0)->GetCoef()) | *(fr.GetCoefRep().at(1)->GetCoef()) | *(fr.GetCoefRep().at(2)->GetCoef()));
				
		//		m->save(outname.value());
		
			
		}else if (doScalarsToVolume.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix Msc=m->getScalars();
			volume<float> Scvol(Msc.Nrows(),1,1);
			for (unsigned int i=0; i<static_cast<unsigned int>(Msc.Nrows());i++)
						Scvol.value(i,0,0)=Msc.element(i,0);
			save_volume(Scvol,outname.value());
			
		}else if (doVolumeToScalars.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			volume<float> Scvol;
			read_volume(Scvol,inname.value());
			// all vertices should be arranged along the x-axis
			ColumnVector Msc(Scvol.xsize());
			for (int i=0; i<Scvol.xsize();i++) {
			  Msc.element(i)=Scvol.value(i,0,0);
			}
			m->setScalars(Msc);
			m->save(outname.value());
			
		}else if (doVectorsToVolume.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix Msc=m->getVectors();
			volume4D<float> Scvol(Msc.Nrows(),1,1,Msc.Ncols());
			for (unsigned int i=0; i<static_cast<unsigned int>(Msc.Nrows());i++) {
			  for (unsigned int j=0; j<static_cast<unsigned int>(Msc.Ncols());j++) {
			    Scvol.value(i,0,0,j)=Msc.element(i,j);
			  }
			}
			save_volume4D(Scvol,outname.value());
			
		}else if (doVolumeToVectors.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			volume4D<float> Scvol;
			read_volume4D(Scvol,inname.value());
			// all vertices should be arranged along the x-axis
			Matrix Msc(Scvol.xsize(),Scvol.tsize());
			for (int i=0; i<Scvol.xsize();i++) {
			  for (int j=0; j<Scvol.tsize();j++) {
			    Msc.element(i,j)=Scvol.value(i,0,0,j);
			  }
			}
			m->setVectors(Msc);
			m->save(outname.value());
			
		}else if (doPointsToVolume.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix Msc=m->getPointsAsMatrix();
			volume<float> Scvol(Msc.Nrows(),3,1);
			for (unsigned int i=0; i<static_cast<unsigned int>(Msc.Nrows());i++) {
			  for (unsigned int j=0; j<3; j++) {
			    Scvol.value(i,j,0)=Msc.element(i,j);
			  }
			}
			save_volume(Scvol,outname.value());
			
		}else if (doFtoP.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix Msc=m->getScalars();
			m->readPolyData(inmeshname.value());
			if (verbose.value()) { cout<<"Msc "<<Msc<<endl; }
			F2z& ftz=F2z::getInstance();
			int df1, df2;
			if (verbose.value()) { m->displayNumericFieldDataNames(); }
			Matrix fdofs=m->getField("FStatDOFs");
			df1=MISCMATHS::round(fdofs(1,1));
			df2=MISCMATHS::round(fdofs(2,1));
			// override dofs with user-specified values (not generally needed)
			if (dof.set()) { df1=dof.value(); }
			if (dof2.set()) { df2=dof2.value(); }
			if (verbose.value()) { cout << "FStat DOFs = " << df1 << " and " << df2 << endl; }
			for (int i=0; i<Msc.Nrows();i++)
			  {
			    if (verbose.value()) {
			      if (abs(Msc.element(i,0))>3)
				cout<<"F "<<Msc.element(i,0)<<" z "<<1-MISCMATHS::fdtr(df1,df2,Msc.element(i,0))<<" p "<<erfc(ftz.convert(Msc.element(i,0),df1,df2))<<endl;
			    }
			    Msc.element(i,0)=1-MISCMATHS::fdtr(df1,df2,Msc.element(i,0));
			  }
			m->setScalars(Msc);
			m->save(outname.value());

			
		}else if (doGetMeans.value()){
			meshUtils *m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix pts=m->getPointsAsMatrix();
			Matrix Vecs=m->getVectors();
			ColumnVector labels(pts.Nrows());
			labels=0;
			
			meshUtils *mout = new meshUtils();
			mout->setPoints(pts);
			mout->setPolygons(m->getPolygons());

		
			Matrix dist=Vecs;
			for (int i =0;i<pts.Nrows();i++)
			{
				labels.element(i)=sqrt(dist.element(i,0)*dist.element(i,0)+dist.element(i,1)*dist.element(i,1)+dist.element(i,2)*dist.element(i,2));
				cout<<labels.element(i)<<endl;
			}

			mout->setScalars(labels);
			mout->save(outname.value()+"_mean0.vtk");
	
			
			labels=1;
				mout->setPoints(pts+Vecs);
			mout->setScalars(labels);
			mout->save(outname.value()+"_mean1.vtk");
		
		}else if (doCoefModelToImage.value()){//used to play around 
				int mode=myindex.value();

				cout<<"replace verts"<<endl;
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
				
				Matrix CoefMean=m->getPointsAsMatrix();
				cout<<"Smodes"<<endl;
				Matrix CoefModes=m->getField("Smodes");
				cout<<"SCondEigs"<<endl;
				ColumnVector se = m->getField("SCondEigs").t();
								ColumnVector ie = m->getField("ICondEigs").t();
				Matrix IModes=m->getField("Imodes");

				ColumnVector Vimean=m->getField("Imean");
				cout<<"i mean "<<Vimean.Nrows()<<" "<<IModes.Nrows()<<endl;
				
				
				
				FnirtFileReader fr(inname.value());
				
				cout<<"coef "<<CoefMean.Nrows()<<" "<<CoefMean.Ncols()<<" "<<CoefModes.Nrows()<<" "<<CoefModes.Ncols()<<" "<<endl;
				ColumnVector NewCoefX(CoefMean.Nrows());
				ColumnVector NewCoefY(CoefMean.Nrows());
				ColumnVector NewCoefZ(CoefMean.Nrows());
				
				volume4D<float> imean;
				volume4D<float> ivar;
				volume4D<float> fmean;
				
				volume<float> imtemp, imY,imZ;
				
				read_volume(imtemp,inname.value());
				imY=imtemp;
				imZ=imtemp;
				int count=0;
				for (int k=0;k<imtemp.zsize();k++)
					for (int j=0;j<imtemp.ysize();j++)
						for (int i=0;i<imtemp.xsize();i++)
						{
							imtemp.value(i,j,k)=CoefMean.element(count,0);
							imY.value(i,j,k)=CoefMean.element(count,1);
							imZ.value(i,j,k)=CoefMean.element(count,2);
							count++;
						}
				imean.addvolume(imtemp);
				imean.addvolume(imY);
				imean.addvolume(imZ);
				//save_volume4D(imean,outname.value());
				fmean=fr.FieldAsNewimageVolume4D();
				save_volume4D(fmean,"field_"+outname.value());
				
				//	meshUtils::getConditionalMeanAndVariance(model1, imean, ivar, imtemp, 0, -3, 3, 0.5, 210);
				
				//save_volume4D(imean, outname.value()+"_mean");
				//			save_volume4D(ivar, outname.value()+"_var");
				
				meshUtils* mout= new meshUtils(inmeshname2.value(),static_cast<meshUtils::DataType>(1));
				mout->warpGridWithDefField(fmean,0,0,0);
				//				mout->setPoints(CoefMean);
				mout->setScalars(Vimean);
				//cout<<"vimena "<<Vimean<<endl;
				mout->save("ugrid_int_mean.vtk");
				vector<float> vars;
				for (unsigned int i=0;i<=static_cast<unsigned int>(mode);i++)
							vars.push_back(0);
				
				volume4D<float> Vmode;
				volume4D<float> VmodeVar;
/*
				Matrix Mivar=m->getField("ICondMat");
				ColumnVector ieigs=m->getField("ICondEigs").t();
				ColumnVector Var(Mivar.Nrows());
				cout<<ieigs.Nrows()<<" "<<Mivar.Nrows()<<" "<<Mivar.Ncols()<<" "<<Var.Nrows()<<endl;
				for (unsigned int i=0;i<Var.Nrows();i++)
				{ 
					float vartemp=0;
					for (unsigned int j=0; j<Mivar.Ncols();j++)
						vartemp+=Mivar.element(i,j)*Mivar.element(i,j)*ieigs.element(j);
					
					Var.element(i)=vartemp;
				
				}
				*/
				for (float i=-3;i<=3;i+=0.5)
				{
				cout<<"Iter i "<<i<<endl;
	
				vars.at(mode)=i;
				Matrix inew=meshUtils::getDeformedVector(Vimean, IModes, se,vars);

				//*************Calculare variance**************//
				float n=336;//number of subjects
				float alpha=n-1/n; //degrees of freedom of joint distribution
				float k2=10584;//number of coefficients
				float den=(alpha+k2-2)*(n-1);
				float sc=(alpha+i*i)/den; //binner is i *i because only varying 1 mode
				cout<<"SCALE "<<sc<<endl;
				//Var*=sc;
				///******************//
				
				NewCoefX=CoefMean.Column(1);
				NewCoefY=CoefMean.Column(2);
				NewCoefZ=CoefMean.Column(3);
				//int mode=0;
				for (int i =0; i<CoefMean.Nrows();i++)
				{
					NewCoefX.element(i)+=CoefModes.element(3*i,mode)*sqrt(se.element(mode))*vars.at(mode);
					NewCoefY.element(i)+=CoefModes.element(3*i+1,mode)*sqrt(se.element(mode))*vars.at(mode);
					NewCoefZ.element(i)+=CoefModes.element(3*i+2,mode)*sqrt(se.element(mode))*vars.at(mode);
					
				}
				
				
				//////-------------------FUCNTION DISAAPPEARED NEED THEM TO WORK PROPERLY ------------------//////
//				fr.GetCoefRep().at(0)->SetCoef(NewCoefX);
//				fr.GetCoefRep().at(1)->SetCoef(NewCoefY);
//				fr.GetCoefRep().at(2)->SetCoef(NewCoefZ);
				volume4D<float> field=fr.FieldAsNewimageVolume4D();
				meshUtils* mout2= new meshUtils(inmeshname2.value(),static_cast<meshUtils::DataType>(1));
				mout2->warpGridWithDefField(field,0,0,0);
				mout2->setScalars(inew);
				cout<<"inew "<<inew.Nrows()<<endl;
				volume<float> image;
				read_volume(image,inname2.value());
				mout2->ugridToImage<float>(image);
				Vmode.addvolume(image);
			/*	
				mout2->setScalars(Var*sc);
				mout2->ugridToImage<float>(image);
				VmodeVar.addvolume(image);
*/
				
				
				//mout2->save("ugrid_int_min3mde0.vtk");
				delete mout2;
				cout<<"end of loop"<<endl;
				}
				cout<<"done looping"<<endl;
				save_volume4D(Vmode,outname.value());
				//				save_volume4D(VmodeVar,outname.value()+"_var");

				//	cout<<"number of ceofficient fields "<<fr.GetCoefRep().size()<<" "<<fr.GetCoefRep().at(0)->GetCoef()->Nrows()<<" "<<fr.GetCoefRep().at(1)->CoefSz()<<endl;
				//	Matrix Coefs=*(fr.GetCoefRep().at(0)->GetCoef()) & *(fr.GetCoefRep().at(1)->GetCoef()) & *(fr.GetCoefRep().at(2)->GetCoef());
				//	cout<<"Coefs "<<Coefs.Nrows()<<" "<<Coefs.Ncols()<<endl;
				//	m->setPoints(*(fr.GetCoefRep().at(0)->GetCoef()) | *(fr.GetCoefRep().at(1)->GetCoef()) | *(fr.GetCoefRep().at(2)->GetCoef()));
				
				//	m->save(outname.value());
			}else if (doFieldModelToImage.value()){//used to play around 
				int mode=myindex.value();

				cout<<"replace verts"<<endl;
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
				
				Matrix CoefMean=m->getPointsAsMatrix();
				cout<<"Smodes"<<endl;
				Matrix CoefModes=m->getField("Smodes");
				cout<<"SCondEigs"<<endl;
				ColumnVector se = m->getField("SCondEigs").t();
				ColumnVector ie = m->getField("ICondEigs").t();
				Matrix IModes=m->getField("Imodes");

				ColumnVector Vimean=m->getField("Imean");
				cout<<"i mean "<<Vimean.Nrows()<<endl;
	
							vector<float> vars;
				for (unsigned int i=0;i<=static_cast<unsigned int>(mode);i++)
							vars.push_back(0);
				
				volume4D<float> Vmode;
				volume4D<float> VmodeVar;

				Matrix Mivar=m->getField("ICondMat");
				ColumnVector ieigs=m->getField("ICondEigs").t();
				ColumnVector Var(Mivar.Nrows());
				cout<<ieigs.Nrows()<<" "<<Mivar.Nrows()<<" "<<Mivar.Ncols()<<" "<<Var.Nrows()<<endl;
				for (unsigned int i=0;i<static_cast<unsigned int>(Var.Nrows());i++)
				{ 
					float vartemp=0;
					for (unsigned int j=0; j<static_cast<unsigned int>(Mivar.Ncols());j++)
						vartemp+=Mivar.element(i,j)*Mivar.element(i,j)*ieigs.element(j);
					
					Var.element(i)=vartemp;
				
				}
			
				meshUtils* mout2= new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
				for (float i=-3;i<=3;i+=0.5)
		//					for (float i=-25;i<=59;i+=10)

					{
				cout<<"Iter i "<<i<<endl;
				vars.at(mode)=i;

				for (unsigned int q=0;q<vars.size();q++)
					cout<<vars.at(q)<<" ";
				cout<<endl;	
	
			//	Matrix snew=meshUtils::getDeformedVector(Vimean, IModes, se,vars);
				Matrix inew=meshUtils::getDeformedVector(Vimean, IModes, se,vars);

				//*************calculare variance**************//
				float n=336;//number of subjects
				float alpha=n-1/n; //degrees of freedom of joint distribution
				float k2=10584;//number of coefficients
				float den=(alpha+k2-2)*(n-1);
				float sc=(alpha+i*i)/den; //binner is i *i because only varying 1 mode
				cout<<"SCALE "<<sc<<endl;
				//Var*=sc;
				///******************//
			
				//int mode=0;
				CoefMean=m->getPointsAsMatrix();
				cout<<CoefMean.element(0,0)<<" "<<CoefMean.element(0,1)<<" "<<CoefMean.element(0,2)<<endl;
				for (int i =0; i<CoefMean.Nrows();i++)
				{
					CoefMean.element(i,0)+=CoefModes.element(3*i,mode)*sqrt(se.element(mode))*vars.at(mode);
					CoefMean.element(i,1)+=CoefModes.element(3*i+1,mode)*sqrt(se.element(mode))*vars.at(mode);
					CoefMean.element(i,2)+=CoefModes.element(3*i+2,mode)*sqrt(se.element(mode))*vars.at(mode);
					
				}
		
			//	meshUtils* mout2= new meshUtils(inmeshname2.value(),static_cast<meshUtils::DataType>(1));
				mout2->setPoints(CoefMean);
				mout2->setScalars(inew);
				
								cout<<"points set "<<endl;

				volume<float> image;
				read_volume(image,inname.value());
				mout2->ugridToImage<float>(image);
				Vmode.addvolume(image);
				
				
				cout<<"now do var "<<endl;
				mout2->setScalars(Var*sc);
								cout<<"now do var2 "<<endl;

				mout2->ugridToImage<float>(image);
				VmodeVar.addvolume(image);
				cout<<"now do var added "<<endl;

				
				
			//	mout2->save("ugrid_int_min3mde0.vtk");
			//	delete mout2;
				cout<<"end of loop"<<endl;
				}
				cout<<"done looping"<<endl;
				save_volume4D(Vmode,outname.value());
				save_volume4D(VmodeVar,outname.value()+"_var");

				//	cout<<"number of ceofficient fields "<<fr.GetCoefRep().size()<<" "<<fr.GetCoefRep().at(0)->GetCoef()->Nrows()<<" "<<fr.GetCoefRep().at(1)->CoefSz()<<endl;
				//	Matrix Coefs=*(fr.GetCoefRep().at(0)->GetCoef()) & *(fr.GetCoefRep().at(1)->GetCoef()) & *(fr.GetCoefRep().at(2)->GetCoef());
				//	cout<<"Coefs "<<Coefs.Nrows()<<" "<<Coefs.Ncols()<<endl;
				//	m->setPoints(*(fr.GetCoefRep().at(0)->GetCoef()) | *(fr.GetCoefRep().at(1)->GetCoef()) | *(fr.GetCoefRep().at(2)->GetCoef()));
				
				//	m->save(outname.value());
				
			}else if (doSubSampleGrid.value()){
				
				cout<<"sub sample grid"<<endl;
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
								cout<<"get Points"<<endl;

				Matrix Pts=m->getPointsAsMatrix();
								cout<<"opoints got "<<Pts.Nrows()<<" "<<Pts.Ncols()<<endl;

				//sample image at points
				volume<short> image;
				vector<short> vsamples;
				read_volume(image,inname.value());
				
				m->sampleImageAtPoints<short>(image, vsamples);
				cout<<"crreate mask"<<endl;
				vector<bool> vmask;
				int N=0;
				for (vector<short>::iterator i=vsamples.begin();i!=vsamples.end();i++)
				{
				
					if ((*i) >0 ) 
					{
						vmask.push_back(true);
						N++;
						cout<<"true "<<*i<<endl;
					}else
					{
						vmask.push_back(false);
												cout<<"false "<<*i<<endl;

					}
				}
				cout<<"set data "<<N<<endl;
				
				meshUtils* mout = new meshUtils();
				mout->setDataType(static_cast<meshUtils::DataType>(1));
								cout<<"Pointss"<<endl;
				mout->setPoints(meshUtils::subSampleMatrix(m->getPointsAsMatrix(),vmask));
								cout<<"Smodes"<<endl;

				mout->addFieldData(meshUtils::subSample_Nby1_3D_Matrix(m->getField("Smodes"),vmask),"Smodes","float");
				mout->addFieldData(meshUtils::subSampleMatrix(m->getField("Imodes"),vmask),"Imodes","float");
				mout->addFieldData(meshUtils::subSampleMatrix(m->getField("Imean"),vmask),"Imean","float");
				mout->addFieldData(meshUtils::subSampleMatrix(m->getField("ICondMat"),vmask),"ICondMat","float");
			
				mout->addFieldData(m->getField("Errs"),"Errs","float");
				mout->addFieldData(m->getField("Labels"),"Labels","float");
				mout->addFieldData(m->getField("NumberOfSubjects"),"NumberOfSubjects","float");
				mout->addFieldData(m->getField("SCondEigs"),"SCondEigs","float");
				mout->addFieldData(m->getField("ICondEigs"),"ICondEigs","float");
				cout<<"saving"<<endl;
				mout->save(outname.value());		
				delete m;
				delete mout;
				
			}else if (doFlipMesh.value()){
				volume<float> im;
				read_volume(im,inname.value());
				float tx=im.xsize()*im.xdim();

				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				m->shiftPoints(-tx,0,0);
				m->scalePoints(-1,1,1);
			//	m->shiftPoints(tx,ty,tz);
				m->save(outname.value());
			}else if (doAppendConstScalar.value()){
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				ColumnVector Sc(m->getPointsAsMatrix().Nrows());
				Sc=label.value();
				m->setScalars(Sc);
				m->save(outname.value());
			}else if (doConcatIntensityGrid.value()){
					meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
					meshUtils* m2 = new meshUtils(inmeshname2.value(),static_cast<meshUtils::DataType>(1));
				
					m->setPoints(m->getPointsAsMatrix() & m2->getPointsAsMatrix() );
					m->setScalars(m->getScalars() & m2->getScalars() );
					m->save(outname.value());
				
					delete m;
					delete m2;	
			}else if (doDeMeanIntensities.value()){
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(1));
				Matrix intensity=m->getScalars();
				cout<<intensity.Nrows()<<" "<<intensity.Ncols()<<" "<<static_cast<unsigned int>(intensity.Nrows())<<endl;
				float avg=0;
				for (unsigned int i=0; i<static_cast<unsigned int>(intensity.Nrows());i++)
					avg+=intensity.element(i,0);//sum values
				
				avg/=intensity.Nrows();//normalize

				for (unsigned int i=0; i<static_cast<unsigned int>(intensity.Nrows());i++)
					intensity.element(i,0)-=avg;//demean
				
				m->setScalars(intensity);	
				m->save(outname.value());
				
				delete m;
			}else if (doDeformSurface.value())
			{
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				volume<float> image;
				read_volume(image, inname.value());
				m->deformSurface<float,unsigned int>(image, dof.value(), w_im.value(), w_tan.value(), w_tri.value(), w_norm.value(),thresh.value(),inmeshname.value());//iteration, image weighting, tangential weighting, max_triangle weighting, normal weigthing
				m->save(outname.value());
				delete m;	
					
			}else if (doMeshReg_LeastSq.value()){
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				meshUtils* mtarg = new meshUtils(inmeshname2.value(),static_cast<meshUtils::DataType>(0));
				Matrix Points_src=m->getPointsAsMatrix();
				
				Matrix fmat=mtarg->reg_leastsq(Points_src, dof.value());
				delete mtarg;

				m->meshReg(fmat);
				m->save((outname.value()+".vtk").c_str());
				ofstream fmatout;
				fmatout.open((outname.value()+".mat").c_str());
				for (int i=0;i<4;i++)
				{
					for (int j=0;j<4;j++)
						fmatout<<fmat.element(i,j)<<" ";
					fmatout<<endl;
				}
				fmatout.close();
				delete m;
			}else if (doGetMeshFromModel.value())
			{
					meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
					meshUtils* mout = new meshUtils();
					mout->setPoints(m->getPointsAsMatrix());
					mout->setPolygons(m->getPolygons());
					mout->save(outname.value());
					delete m;
					delete mout;
			}else if (doVertexLDA_LOO.value())
			{
				
				//load target
				ifstream ftarg;
				ftarg.open(inname.value().c_str());
				int temp;
				unsigned int count=0;
				while (ftarg>>temp)
					count++;
				ftarg.close();	
				
				ColumnVector target(count);
				ftarg.open(inname.value().c_str());
				count=0;
				while (ftarg>>temp)
				{
					target.element(count)=temp;
					cout<<"target "<<temp<<endl;
					count++;
				}
				ftarg.close();	
				
				//base is usually average mesh
				meshUtils* mbase = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				
	//creates mean target mesh from data
//				Matrix alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),"nosave");
//				for (int i=0;i<5;i++)
//				{
//					
//					if (i>0)
//						alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),"nosave");
//					
//					
//					Matrix meanPts(mbase->getPointsAsMatrix().Nrows(),3);
//					for (unsigned int i=1;i<static_cast<unsigned int>(alignedPoints.Nrows());i+=3)
//					{
//						MVdisc* vertDisc = new MVdisc();
//						Matrix m=(vertDisc->getGroupMeans(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target)).Column(1);					
//						//cout<<"i "<<i<<" "<<m.Nrows()<<" "<<m.Ncols()<<endl;
//						meanPts.Row((i-1)/3+1)=(vertDisc->getGroupMeans(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target)).Column(1).t();
//						delete vertDisc;
//					}
//					mbase->setPoints(meanPts);
//					
//				}
				
				
				Matrix alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),outname.value()+"_aligned.vtk");
				
				//do for each vertex 
				vector<float> v_accuracy;
				for (unsigned int i=1;i<static_cast<unsigned int>(alignedPoints.Nrows());i+=3)
				{
					MVdisc* vertDisc = new MVdisc();					
					//Matrix test=alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols());
					//	vertDisc->estimateLDAParams(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target);
					Matrix discData=alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols());
					if (toggle.value()) //use neighbouring vertices as well
					{
						vector< vector<unsigned int> > neigh = first_mesh::findNeighbours<unsigned int>(mbase->getPolygonsAsVectorOfVectors<unsigned int>(),alignedPoints.Nrows()/3);
						for (unsigned int j=0; j<neigh.at(i).size();j++)
							discData=discData & alignedPoints.SubMatrix(neigh.at(i).at(j)*3,neigh.at(i).at(j)*3+2,1,alignedPoints.Ncols());
					}
					cout<<"discData "<<discData.Nrows()<<" "<<discData.Ncols()<<endl;
					float accuracy = vertDisc->run_LOO_LDA(discData,target);
					cout<<"accuracy "<<accuracy<<endl;
					v_accuracy.push_back(accuracy);	
					delete vertDisc;
				}
				mbase->setScalars<float>(v_accuracy);
				mbase->save(outname.value());
			}else if (doVertexLDA_save.value())
			{
						
				//load target
				ifstream ftarg;
				ftarg.open(inname.value().c_str());
				int temp;
				unsigned int count=0;
				while (ftarg>>temp)
					count++;
				ftarg.close();	
				
				ColumnVector target(count);
				ftarg.open(inname.value().c_str());
				count=0;
				while (ftarg>>temp)
				{
					target.element(count)=temp;
					cout<<"target "<<temp<<endl;
					count++;
				}
				ftarg.close();	
			
				//base is usually average mesh
				meshUtils* mbase = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
//				Matrix alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),"nosave");
//				for (int i=0;i<5;i++)
//				{
//					
//					if (i>0)
//						alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),"nosave");
//					
//					
//					Matrix meanPts(mbase->getPointsAsMatrix().Nrows(),3);
//					for (unsigned int i=1;i<static_cast<unsigned int>(alignedPoints.Nrows());i+=3)
//					{
//						MVdisc* vertDisc = new MVdisc();
//						Matrix m=(vertDisc->getGroupMeans(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target)).Column(1);					
//						//cout<<"i "<<i<<" "<<m.Nrows()<<" "<<m.Ncols()<<endl;
//						meanPts.Row((i-1)/3+1)=(vertDisc->getGroupMeans(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target)).Column(1).t();
//						delete vertDisc;
//					}
//					mbase->setPoints(meanPts);
//					
//				}
				
//				cout<<"done looping align"<<endl;
				Matrix alignedPoints=mbase->alignSurfaces(inmeshname2.value(),dof.value(),outname.value()+"_aligned.vtk");

				//do for each vertex 
//				vector<float> v_accuracy;
				MVdisc* vertDisc = new MVdisc();					

				for (unsigned int i=1;i<static_cast<unsigned int>(alignedPoints.Nrows());i+=3)
				{
					//Matrix test=alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols());
				//	vertDisc->estimateLDAParams(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target);
					Matrix discData=alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols());
					if (toggle.value()) //use neighbouring vertices as well
					{
						vector< vector<unsigned int> > neigh = first_mesh::findNeighbours<unsigned int>(mbase->getPolygonsAsVectorOfVectors<unsigned int>(),alignedPoints.Nrows()/3);
						for (unsigned int j=0; j<neigh.at(i).size();j++)
							discData=discData & alignedPoints.SubMatrix(neigh.at(i).at(j)*3,neigh.at(i).at(j)*3+2,1,alignedPoints.Ncols());
					}
				cout<<"discData "<<discData.Nrows()<<" "<<discData.Ncols()<<endl;
				if (i==1)
					vertDisc->estimateLDAParams(discData,target);
				else
					vertDisc->estimateAndAppendLDAParams(discData,target);
				//	cout<<"accuracy "<<accuracy<<endl;
				//	 v_accuracy.push_back(accuracy);	
				//	 delete vertDisc;
				}
			//	mbase->setScalars<float>(v_accuracy);
				vertDisc->saveLDAParams(outname.value(),mbase->getPolygons());
				
				delete vertDisc;
				delete mbase;
				
			}else if (doVertexLDA_loadAndApply.value())
			{
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				Matrix Mean1_all=m->getPointsAsMatrix();
				Matrix Mean2_all=m->getField("mean2");
				Matrix Cov_vecs_all=m->getField("Covariance_EigVecs");
				Matrix Cov_vals_all=m->getField("Covariance_EigVals");
				vector<unsigned int> nsub_all=first_newmat_vector::vectorToVector<unsigned int>(m->getField("number_of_subjects"));
				
				Matrix alignedPoints=m->alignSurfaces(inmeshname2.value(),dof.value(),outname.value()+"_aligned.vtk");

				//unsigned int subject=1;
				vector<int> v_class;
				fslvtkIO* fout = new fslvtkIO();
				
				for (unsigned int subject=1; subject<=(unsigned)alignedPoints.Ncols() ;subject++)
				{
					//do for each vertex
					int p_count=1; 
					for (unsigned int i=1;i<static_cast<unsigned int>(alignedPoints.Nrows());i+=3,p_count++)
					{
						MVdisc* vertDisc = new MVdisc();					
						//Matrix test=alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols());
						//	vertDisc->estimateLDAParams(alignedPoints.SubMatrix(i,i+2,1,alignedPoints.Ncols()),target);
						//cout<<"i apply "<<i<<" "<<p_count<<" "<<Mean2_all.Nrows()<<" "<<Mean2_all.Ncols()<<" "<<Cov_vecs_all.Nrows()<<" "<<Cov_vecs_all.Nrows()<<" "<<endl;
						
						vertDisc->set_LDA_Params( ( Mean1_all.SubMatrix(p_count,p_count,1,3).t() | Mean2_all.SubMatrix(p_count,p_count,1,3).t() ) ,\
												  Cov_vecs_all.SubMatrix(i,i+2,1,3), \
												  first_newmat_vector::vectorToVector<float>( Cov_vals_all.SubMatrix(i,i+2,1,1) ), \
												  nsub_all );
						ColumnVector discData=alignedPoints.SubMatrix(i,i+2,subject,subject);
						v_class.push_back( static_cast<int>(vertDisc->applyLDA(discData, 0.0 )) );						
						
						delete vertDisc;
					}
					
					if (subject==1)
					{
						fout->setPoints(alignedPoints.Column(subject));
						fout->setPolygons(m->getPolygons());
					}else
					{
						fout->appendPointsAndPolygons( first_newmat_vector::wrapMatrix(alignedPoints.Column(subject)), m->getPolygons());
					}
				}
				fout->setScalars(v_class);

				fout->save(outname.value());
				delete fout;
				delete m;
			//	mbase->setScalars<float>(v_accuracy);
				//vertDisc->saveLDAParams(outname.value());
			}else if (doGetMaxScalar.value())
			{
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				cout<<m->maxScalar()<<endl;
				delete m;
			}else if (doGetMeanScalar.value())
			{
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				cout<<m->meanScalar()<<endl;
				delete m;
				
			}else if (doDisplayNumericField.value())
			{
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				m->displayNumericField(inname.value());
				delete m;
				
			}else if (doDisplayNumericFieldNames.value())
			{
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				m->displayNumericFieldDataNames();
				delete m;
			}else{
			
				//load target
				ifstream ftarg;
				ftarg.open(inname.value().c_str());
				int temp;
				unsigned int count=0;
				while (ftarg>>temp)
					count++;
				ftarg.close();	
				
				ColumnVector target(count);
				ftarg.open(inname.value().c_str());
				count=0;
				while (ftarg>>temp)
				{
					target.element(count)=temp;
					cout<<"target "<<temp<<endl;
					count++;
				}
				ftarg.close();	
				
				
				meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
				Matrix Sc=m->getScalars();
		
				unsigned int npts=642;
						Matrix Sc_rate=Sc.SubMatrix(1,npts,1,1);
				Sc_rate=0;
				for (unsigned int j=0;j<(Sc.Nrows()/npts) ; j++)
				{
					float frac=0;
					for (unsigned int i=0; i<npts;i++)
					{
						frac+=Sc.element(i+j*npts,0);
						if (Sc.element(i+j*npts,0) ==target.element(j))
							Sc_rate.element(i,0)++;
					}
					cout<<"j frac "<<j<<" "<<frac/npts<<endl;
				}
				
				Matrix tempM;
				tempM=m->getPointsAsMatrix().SubMatrix(1,npts,1,3);
				m->setPoints(tempM);
				m->setScalars(Sc_rate/(Sc.Nrows()/npts));
				m->save(outname.value());
				delete m;
			
			}
			/*else if (doAddModesUsingScalars.value())
			{
			cout<<"sample grid"<<endl;
			
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix sc=m->getScalars();
			vector<bool> vsc;
			for (int i=0;i<sc.Nrows();i++)
			{
				if (sc.element(i,0)>thresh.value()){
				vsc.push_back(true);
				//						vsc.push_back(false);
			}else
					vsc.push_back(false);
			}
			shapeModel* model1 = new shapeModel;
			model1->load_bmv_binaryInfo(inname.value(),1);
			model1->load_bmv_binary(inname.value(),1);	
	
			meshUtils::addModesToModelUsingMask(model1, vsc);
						cout<<"new model "<<model1->getNumberOfModes()<<endl;

			model1->save_binary(outname.value(),0,model1->getNumberOfModes());
			cout<<"new model "<<model1->getNumberOfModes()<<endl;
			delete m; 
			delete model1;
			
			
		}else if (doWriteConditionalIntensity.value())
		{
									
			shapeModel* model1 = new shapeModel;
			model1->load_bmv_binaryInfo(inmeshname.value(),1);
			model1->load_bmv_binary(inmeshname.value(),1);	
			model1->setImageParameters(182,218,182, 1.0, 1.0,1.0);

			volume4D<float> imean;
			volume4D<float> ivar;
			volume<float> imtemp;
			read_volume(imtemp,inname.value());
			
			meshUtils::getConditionalMeanAndVariance(model1, imean, ivar, imtemp, 0, -3, 3, 0.5, 210);

			save_volume4D(imean, outname.value()+"_mean");
						save_volume4D(ivar, outname.value()+"_var");

			delete model1;
			
			
		}else if (doMeshToBvars.value()){
			vector<float> vars;
			shapeModel* model1 = new shapeModel;
			model1->load_bmv_binaryInfo(inname.value(),1);
			model1->load_bmv_binary(inname.value(),1);	
			
			model1->setImageParameters(182,218,182, 1.0, 1.0,1.0);
			Mesh m2=model1->getTranslatedMesh(0);
					
			meshUtils* m = new meshUtils(inmeshname.value(),static_cast<meshUtils::DataType>(0));
			Matrix vpts=m->getPointsAsMatrix();
			int count=0;
				for (vector<Mpoint*>::iterator i = m2._points.begin(); i!=m2._points.end(); i++ , count++)
			{
				cout<<(*i)->get_coord().X<<" "<<(*i)->get_coord().Y<<" "<<(*i)->get_coord().Z<<endl;
				vpts.element(count,0)-=(*i)->get_coord().X;
				vpts.element(count,1)-=(*i)->get_coord().Y;
				vpts.element(count,2)-=(*i)->get_coord().Z;
			

				}
			cout<<vpts<<endl;
			//vars=m->getPointsAsVector<float>();
			for (int i=0;i<model1->getNumberOfModes();i++)
			{
				float var=0;
				vector<Vec> smode=model1->getShapeMode(0,i);
				for (unsigned int j=0;j<smode.size() ; j++)
				{
					var+=smode.at(j).X*vpts.element(j,0)+smode.at(j).Y*vpts.element(j,1)+smode.at(j).Z*vpts.element(j,2);
					
				}
				vars.push_back(var/model1->getEigenValue(i));
				
				//subtract the mode from the points
				for (unsigned int j=0;j<smode.size() ; j++)
				{
				vpts.element(j,0)-=vars.at(i)*model1->getEigenValue(i)*smode.at(j).X ;
								vpts.element(j,1)-=vars.at(i)*model1->getEigenValue(i)*smode.at(j).Y;
				vpts.element(j,2)-=vars.at(i)*model1->getEigenValue(i)*smode.at(j).Z ;

				}
				
				
			//	cout<<"MODE "<<var<<" "<<model1->getEigenValue(i)<<" "<<var/model1->getEigenValue(i)<<endl;
			}
			ofstream fout;
			fout.open(outname.value().c_str());
			fout<<"this is a bvars file "<<endl;
			fout<<"L_Thal_bin.bmv "<<endl;
fout<<"NumberOfSubjects 1"<<endl;
fout<<"group1 "<<model1->getNumberOfModes()<<" ";
				for (unsigned int i=0;i<vars.size() ; i++)
					fout<<vars.at(i)<<" ";
			fout<<endl;
		
		}*/
			}catch(exception& e){
				
				cout<<e.what()<<endl;
				return 1;
			}
			
		}catch(X_OptionError& e) {
	options.usage();
		cerr << endl << e.what() << endl;
			exit(EXIT_FAILURE);
		} catch(std::exception &e) {
			cerr << e.what() << endl;
		} 
		
  return 0;// do_work(argc,argv);
}


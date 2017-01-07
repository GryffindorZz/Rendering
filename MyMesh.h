//
// Created by Gryffindor on 2017/1/6.
//

#ifndef RENDERING_MYMESH_H
#define RENDERING_MYMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct MyTraits:public OpenMesh::DefaultTraits
{
    VertexTraits{
    //
    };
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

#endif //RENDERING_MYMESH_H

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkCleanPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <stdio.h>
#include <array>
#include <random>
#include <vtkLookupTable.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>
#include "gate_generator.h"

vtkSmartPointer<vtkActor> piSphereActor() {
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetCenter(0, 0, 0);
    sphere->SetRadius(M_PI);
    sphere->SetPhiResolution(256);
    sphere->SetThetaResolution(256);
    sphere->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    actor->GetProperty()->SetOpacity(0.3);
    return actor;
}

#define MIN_DEPTH 1
#define MAX_DEPTH 19
#define DEFAULT_DEPTH MAX_DEPTH

void fill(vtkSmartPointer<vtkPoints> points, const std::vector<std::vector<Eigen::Matrix2cd> *> &gates, int depth) {
    for (int i = 0; i < depth; i++) {
        auto gs = gates[i];
        for (const auto &g: *gs) {
            auto[x, y, z] = vectorize(g);

            points->InsertNextPoint(x, y, z);
        }
    }
}

int main() {
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.9, 0.9, 0.9);

    renderer->AddActor(piSphereActor());

    auto seeds = getSeeds();
    const std::vector<std::vector<Eigen::Matrix2cd> *> &gates = *generateCompositeGates(MAX_DEPTH, &seeds);

    vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();

    fill(points, gates, DEFAULT_DEPTH);

    auto pointsPolydata =
            vtkSmartPointer<vtkPolyData>::New();

    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter =
            vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    auto polydata =
            vtkSmartPointer<vtkPolyData>::New();
    polydata->ShallowCopy(vertexFilter->GetOutput());


    auto lookupTable =
            vtkSmartPointer<vtkLookupTable>::New();
    lookupTable->SetTableRange(0.0, gates.size() - 1);
    lookupTable->Build();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(3);
    actor->GetProperty()->SetColor(0, 0, 0);

    renderer->AddActor(actor);

    renderer->ResetCamera();


    vtkSmartPointer<vtkSliderRepresentation2D> sliderRep =
            vtkSmartPointer<vtkSliderRepresentation2D>::New();

    sliderRep->SetMinimumValue(MIN_DEPTH);
    sliderRep->SetMaximumValue(MAX_DEPTH);
    sliderRep->SetValue(DEFAULT_DEPTH);
    sliderRep->SetTitleText("number of gates");
    sliderRep->GetTitleProperty()->SetColor(0, 0, 0);
    sliderRep->GetLabelProperty()->SetColor(0, 0, 0);
    sliderRep->GetSliderProperty()->SetColor(0.5, 0.5, 0.5);
    sliderRep->GetCapProperty()->SetColor(0.2, 0.2, 0.2);
    sliderRep->GetTubeProperty()->SetColor(0.2, 0.2, 0.2);
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint1Coordinate()->SetValue(100, 50);
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint2Coordinate()->SetValue(500, 50);


    auto renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(600, 600);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Unitary Matrices");

    auto renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    auto sliderWidget =
            vtkSmartPointer<vtkSliderWidget>::New();
    sliderWidget->SetInteractor(renderWindowInteractor);
    sliderWidget->SetRepresentation(sliderRep);
    sliderWidget->SetAnimationModeToAnimate();
    sliderWidget->EnabledOn();

    class sliderCallback : public vtkCommand {
    public:
        static sliderCallback *New() {
            return new sliderCallback;
        }

        void Execute(vtkObject *caller, unsigned long, void *) override {
            auto *sliderWidget =
                    reinterpret_cast<vtkSliderWidget *>(caller);
            auto *sliderRepr = reinterpret_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation());
            int val = static_cast<int>(sliderRepr->GetValue() + 0.5);
            sliderRepr->SetValue(val);
            if (val != last) {
                std::cout << "event executed = " << val << std::endl;
                pointsSet->Reset();
                fill(pointsSet, *gates, val);
                pointsSet->Modified();
                last = val;
            }
        }

        sliderCallback() : pointsSet(nullptr) {}

        vtkPoints *pointsSet;
        int last = -1;
        const std::vector<std::vector<Eigen::Matrix2cd> *> *gates;
    };

    auto callback =
            vtkSmartPointer<sliderCallback>::New();
    callback->pointsSet = points;
    callback->gates = &gates;

    sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);

    auto interactorStyle =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(interactorStyle);

    renderWindowInteractor->Start();
    return 0;
}

#pragma once

#include "common.h"
#include "formula.h"
#include <unordered_set>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;
    bool IsReferenced() const;

private:
    class Impl {
    public:
        virtual ~Impl() = default;
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const = 0;
    	virtual bool IsCacheValid() const { return true; }
    	virtual void InvalidateCache() {}
    };
    
    class EmptyImpl : public Impl {
    public:
    	Value GetValue() const override { return ""; }
    	std::string GetText() const override { return ""; }
    };
    
    class TextImpl : public Impl {
    public:
        explicit TextImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;
        
    private:
        std::string text_;
    };
    
    class FormulaImpl : public Impl {
    public:
        explicit FormulaImpl(std::string text, const SheetInterface& sheet_interface);
    	Value GetValue() const override;
    	std::string GetText() const override;
    	bool IsCacheValid() const override;
    	void InvalidateCache() override;
    	std::vector<Position> GetReferencedCells() const;
        
    private:
    	std::unique_ptr<FormulaInterface> formula_ptr_;
    	const SheetInterface& sheet_interface_;
    	mutable std::optional<FormulaInterface::Value> cache_;
    };
    // проверка наличия циклических зависимостей
    bool FindCyclicDependencies(const Impl& impl) const;
    void InvalidateCache(bool force = false);

private:
    std::unique_ptr<Impl> impl_;
    Sheet& sheet_;
    std::unordered_set<Cell*> l_nodes_;
    std::unordered_set<Cell*> r_nodes_;
};